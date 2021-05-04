// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GIGAMONKEY_VAR_INT_H
#define GIGAMONKEY_VAR_INT_H
#include <data/iterable.hpp>
#include <data/encoding/endian.hpp>
#include <ostream>

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

        /**
         * Equality operator
         * @param rhs VarInt to check if equals
         * @return true if equals false otherwise
         */
        bool operator==(const VarInt &rhs) const;

        /**
         * Inequality operator
         * @param rhs VarInt to check if different
         * @return false if equals true otherwise
         */
        bool operator!=(const VarInt &rhs) const;

        /**
         * Less than operator
         * @param rhs VarInt to check if less than
         * @return true of less then rhs false otherwise
         */
        bool operator<(const VarInt &rhs) const;

        /**
         * Greater than operator
         * @param rhs VarInt to check if greater than
         * @return true of greater then rhs false otherwise
         */
        bool operator>(const VarInt &rhs) const;

        /**
         * Less than or equal operator
         * @param rhs VarInt to check if less or equal than
         * @return true if less then  or equal to rhs false otherwise
         */
        bool operator<=(const VarInt &rhs) const;

        /**
         * Greater than or equal operator
         * @param rhs VarInt to check if greater or equal than
         * @return true if greater then  or equal to rhs false otherwise
         */
        bool operator>=(const VarInt &rhs) const;

        friend std::ostream &operator<<(std::ostream &os, const VarInt &anInt);

    private:
        data::uint64_little value;

    };
}
#endif //GIGAMONKEY_VAR_INT_H
