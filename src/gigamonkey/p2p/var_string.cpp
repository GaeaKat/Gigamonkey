// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gigamonkey/p2p/var_string.hpp>
namespace Gigamonkey::p2p {


    VarString::VarString(data::bytes::iterator iterator) : size(iterator) {
        data::bytes str=data::bytes((uint64_t )size);
        std::copy(iterator+1,iterator+(uint64_t)size+1,str.begin());
        value=std::string(str.begin(),str.end());
    }

    VarString::operator data::bytes() const {
        data::bytes output=data::bytes(size.size()+value.size());
        data::bytes siz=(data::bytes)size;
        std::copy(siz.begin(),siz.end(),output.begin());
        std::copy(value.begin(), value.end(),output.begin()+siz.size());
        return output;
    }

    VarString::operator std::string() const {
        return value;
    }

    uint64_t VarString::getSize() const {
        return size.size()+value.size();
    }
    std::ostream &operator<<(std::ostream &os, const VarString &string) {
        os << "VarString<" << "value: " << string.value << ">";
        return os;
    }
}
