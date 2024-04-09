// Copyright (c) 2024 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#include <gigamonkey/pay/SVP_envelope.hpp>

namespace Gigamonkey::nChain {
    using namespace Bitcoin;

    enum class no_excluded_middle : byte {no = false, yes = true, unknown = 2};

    bool validate_node (
        const Bitcoin::txid &txid,
        const SPV_envelope::node &n,
        no_excluded_middle &MAPI_responses_included,
        const SPV::database *h) {

        // proof xor input nodes must be included.
        if ((bool (n.Proof)) == (data::size (n.Inputs) > 0)) return false;

        // txid must match raw transaction.
        if (Bitcoin::transaction::id (n.RawTx) != txid) return false;

        bool has_MAPI = data::size (n.MAPIResponses) > 0;
        if (MAPI_responses_included == no_excluded_middle::unknown)
            MAPI_responses_included = static_cast<no_excluded_middle> (has_MAPI);
        else if (no_excluded_middle (has_MAPI) != MAPI_responses_included) return false;

        // check signatures on MAPI responses.
        if (!data::valid (n.MAPIResponses)) return false;

        if (bool (n.Proof)) return h != nullptr ? n.Proof->validate (*h) : n.Proof->valid ();

        for (const entry<Bitcoin::txid, ptr<SPV_envelope::node>> &p : n.Inputs)
            if (p.Value == nullptr || !validate_node (p.Key, *p.Value, MAPI_responses_included, h)) return false;

        return true;
    }

    bool validate (const SPV_envelope &n, const SPV::database *h) {
        // if TXID is included, must match tx.
        if (bool (n.TXID) && Bitcoin::transaction::id (n.RawTx) != *n.TXID) return false;

        // check signatures on MAPI responses.
        if (!data::valid (n.MAPIResponses)) return false;

        no_excluded_middle MAPI_responses_included = no_excluded_middle::unknown;

        for (const entry<Bitcoin::txid, SPV_envelope::node> &p : n.Inputs)
            if (!validate_node (p.Key, p.Value, MAPI_responses_included, nullptr)) return false;

        return true;
    }

    bool SPV_envelope::valid () const {
        return nChain::validate (*this, nullptr);
    }

    // validate means that we actually check all the merkle
    // against the block headers.
    bool SPV_envelope::validate (const SPV::database &h) const {
        return nChain::validate (*this, &h);
    }

    SPV_envelope::node to_SPV_envelope (const Bitcoin::txid &txid, const SPV::proof::node &u) {

        if (std::holds_alternative<SPV::proof::confirmation> (u.Proof)) {
            const auto &conf = std::get<SPV::proof::confirmation> (u.Proof);
            return SPV_envelope::node {u.Transaction, proofs_serialization_standard {Merkle::branch {txid, conf.Path}, conf.Header}};
        }

        map<Bitcoin::txid, ptr<SPV_envelope::node>> inputs;
        for (const auto &e : std::get<map<Bitcoin::txid, ptr<SPV::proof::node>>> (u.Proof))
            inputs = inputs.insert (e.Key, std::make_shared<SPV_envelope::node> (to_SPV_envelope (e.Key, *e.Value)));

        return SPV_envelope::node {u.Transaction, inputs};
    }

    SPV_envelope::SPV_envelope (const SPV::proof &u) {

        RawTx = u.Transaction;

        for (const auto &e : u.Proof) Inputs = Inputs.insert (e.Key, to_SPV_envelope (e.Key, e.Value));
    }

    JSON write_txid (const Bitcoin::txid &);
    maybe<Bitcoin::txid> read_txid (const JSON &);

    JSON write_JSON (const SPV_envelope::node &n) {
        JSON::object_t node {};
        node["rawTx"] = encoding::hex::write (n.RawTx);

        if (data::size (n.MAPIResponses) > 0) {
            JSON::array_t mapi_responses {};
            for (const MAPI::transaction_status_response &r : n.MAPIResponses) mapi_responses.push_back (JSON (r));
            node["mapiResponses"] = mapi_responses;
        }

        if (n.Proof) {
            node["proof"] = encoding::hex::write (bytes (*n.Proof));
            return node;
        }

        JSON::object_t inputs {};
        for (const entry<Bitcoin::txid, ptr<SPV_envelope::node>> &p : n.Inputs)
            inputs[write_txid (p.Key)] = write_JSON (*p.Value);
        node["inputs"] = inputs;

        return node;
    }

    SPV_envelope::operator JSON () const {

        JSON::object_t node {};
        if (TXID) node["txid"] = write_txid (*TXID);
        node["rawTx"] = encoding::hex::write (RawTx);

        if (data::size (MAPIResponses) > 0) {
            JSON::array_t mapi_responses {};
            for (const MAPI::transaction_status_response &r : MAPIResponses) mapi_responses.push_back (JSON (r));
            node["mapiResponses"] = mapi_responses;
        }

        JSON::object_t inputs {};
        for (const entry<Bitcoin::txid, SPV_envelope::node> &p : Inputs)
            inputs[write_txid (p.Key)] = write_JSON (p.Value);
        node["inputs"] = inputs;

        return node;
    }

    maybe<list<MAPI::transaction_status_response>> read_mapi_responses (const JSON &j) {
        list<MAPI::transaction_status_response> mapi_responses {};
        if (j.contains ("mapiResponses")) {
            const auto &mapiResponses = j["mapiResponses"];
            if (!mapiResponses.is_array ()) return {};
            for (const auto &jj : mapiResponses) {
                MAPI::transaction_status_response response {jj};
                if (!response.valid ()) return {};
                mapi_responses <<= response;
            }
        }
        return mapi_responses;
    }

    SPV_envelope::node *read_JSON (const JSON &j) {
        if (!j.contains ("rawTx") || (j.contains ("proof") == j.contains ("inputs"))) return nullptr;

        const auto &raw = j["rawTx"];
        if (!raw.is_string ()) return nullptr;
        maybe<bytes> raw_bytes = encoding::hex::read (std::string (raw));
        if (!bool (raw_bytes)) return nullptr;

        auto mapiResponses = read_mapi_responses (j);
        if (!bool (mapiResponses)) return nullptr;

        if (j.contains ("proof")) {
            const auto &proof = j["proof"];
            if (!proof.is_string ()) return nullptr;
            maybe<bytes> raw_proof = encoding::hex::read (std::string (proof));
            if (!bool (raw_proof)) return nullptr;
            auto p = proofs_serialization_standard::read_binary (*raw_proof);
            if (!p.valid ()) return nullptr;
            return new SPV_envelope::node {*raw_bytes, p, *mapiResponses};
        }

        const auto &inputs = j["inputs"];
        if (!inputs.is_object ()) return nullptr;

        map<Bitcoin::txid, ptr<SPV_envelope::node>> Inputs;
        for (const auto &[key, value] : inputs.items ()) {
            SPV_envelope::node *nn = read_JSON (value);
            if (nn == nullptr) return nullptr;
            auto id = read_txid (key);
            if (!bool (id)) return nullptr;
            Inputs = Inputs.insert (*id, ptr<SPV_envelope::node> {nn});
        }

        return new SPV_envelope::node {*raw_bytes, Inputs, *mapiResponses};
    }

    SPV_envelope::SPV_envelope (const JSON &j) {
        if (!j.contains ("rawTx") || !j.contains ("inputs")) return;

        const auto &raw = j["rawTx"];
        if (!raw.is_string ()) return;
        maybe<bytes> raw_bytes = encoding::hex::read (std::string (raw));
        if (!bool (raw_bytes)) return;

        const auto &inputs = j["inputs"];
        if (!inputs.is_object ()) return;

        auto mapiResponses = read_mapi_responses (j);
        if (!bool (mapiResponses)) return;

        map<Bitcoin::txid, node> ins;
        for (const auto &[key, value] : inputs.items ()) {
            SPV_envelope::node *nn = read_JSON (value);
            if (nn == nullptr) return;
            auto id = read_txid (key);
            if (!bool (id)) return;
            ins = ins.insert (*id, *nn);
            delete nn;
        }

        if (j.contains ("txid")) {
            auto id = read_txid (j["txid"]);
            if (!bool (id)) return;
            TXID = *id;
        }

        RawTx = *raw_bytes;

        MAPIResponses = *mapiResponses;
        Inputs = ins;
    }

}