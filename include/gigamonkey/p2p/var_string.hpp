// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GIGAMONKEY_VAR_STRING_H
#define GIGAMONKEY_VAR_STRING_H
#include <data/iterable.hpp>
#include <data/encoding/endian.hpp>
#include <ostream>
#include "var_int.hpp"

namespace Gigamonkey::p2p {

    /**
     * Variable length String.
     */
    class VarString {
    public:
        /**
         * Constructs a VarString from an iterator of bytes of unknown length
         * @param iterator iterator to a vector of bytes
         */
        explicit VarString(data::bytes::iterator iterator);

        /**
         * Constructs a VarString from a vector of bytes.
         * @param cur vector of bytes
         */
        explicit VarString(data::bytes cur): VarString(cur.begin()) {};

        /**
         * Constructs a VarString from a string
         * @param val string value
         */
        explicit VarString(const std::string& val) : value(val),size(val.size()) {};

        /**
         * Casts the VarString to a bytes array of appropriate size
         * @return byte array of variable size
         */
        explicit operator data::bytes() const;

        /**
         * Casts the VarString to a string
         * @return string value of the VarString
         */
        explicit operator std::string() const;


        /**
         * gets the size of the varstring in total
         * @return size of the varstring
         */
        uint64_t getSize() const;

        friend std::ostream &operator<<(std::ostream &os, const VarString &string);

    private:
        std::string value;
        VarInt size;

    };
}
#endif //GIGAMONKEY_VAR_STRING_H
