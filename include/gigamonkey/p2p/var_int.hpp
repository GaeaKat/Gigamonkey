// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GIGAMONKEY_VAR_INT_H
#define GIGAMONKEY_VAR_INT_H
#include <data/iterable.hpp>
#include <data/encoding/endian.hpp>

namespace Gigamonkey::p2p {
    /**
     * Variable Int, Known as CompactSize in protocol Documentation
     */
    class VarInt {
    public:
        /**
         * Constructs a VarInt from an iterator of bytes of unknown length
         * @param iterator iterator to a vector of bytes
         */
        explicit VarInt(data::bytes::iterator iterator);

        /**
         * Constructs a VarInt from a vector of bytes.
         * @param cur Vector of bytes/
         */
        explicit VarInt(data::bytes cur) : VarInt(cur.begin()) {};

        /**
         * Constructs a VarInt from a uint.
         * @param val Value of the VarInt
         */
        explicit VarInt(uint64_t val) : value(val) {};

        /**
         * Size of the VarInt including indicator byte
         * @return 1, 3, 5 or 9
         */
        [[nodiscard]] uint16_t size() const;

        /**
         * Casts the VarInt to a bytes array of appropriate size
         * @return byte array of set size
         */
        explicit operator data::bytes() const;

        /**
         * Casts the value of the VarInt to a uint8
         * @return uint8 containing the value of the VarInt
         */
        explicit operator uint8_t() const;

        /**
         * Casts the value of the VarInt to a uint16
         * @return uint16 containing the value of the VarInt
         */
        explicit operator uint16_t() const;

        /**
         * Casts the value of the VarInt to a uint32
         * @return uint32 containing the value of the VarInt
         */
        explicit operator uint32_t() const;

        /**
         * Casts the value of the VarInt to a uint64
         * @return uint64 containing the value of the VarInt
         */
        explicit operator uint64_t() const;
    private:
        data::uint64_little value;

    };
}
#endif //GIGAMONKEY_VAR_INT_H
