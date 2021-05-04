// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gigamonkey/p2p/var_int.hpp>

namespace Gigamonkey::p2p {

    VarInt::VarInt(data::bytes::iterator iterator) {
        data::byte indicator=*iterator;
        if(indicator < 0xFD)
            value=indicator;
        else if(indicator == 0xFD) {
            char two[2];
            std::copy(iterator+1,iterator+3,std::begin(two));
            // todo: use endian properly
            auto* twoPointer= reinterpret_cast<uint16_t *>(&two);
            value=boost::endian::little_to_native(*twoPointer);
        }
        else if(indicator == 0xFE) {
            char four[4];
            std::copy(iterator+1,iterator+5,std::begin(four));
            // todo: use endian properly
            auto* fourPointer= reinterpret_cast<uint16_t *>(&four);
            value=boost::endian::little_to_native(*fourPointer);
        }
        else if(indicator == 0xFF) {
            char eight[4];
            std::copy(iterator+1,iterator+9,std::begin(eight));
            // todo: use endian properly
            auto* eightPointer= reinterpret_cast<uint16_t *>(&eight);
            value=boost::endian::little_to_native(*eightPointer);
        }
    }

    uint16_t VarInt::size() const {
        if(value<0xFD)
            return 1;
        if(value<=0xFFFF)
            return 3;
        if(value<=0xFFFFFFFF)
            return 5;
        return 9;
    }

    VarInt::operator data::bytes() const {
        uint16_t siz=size();
        data::bytes output=data::bytes(siz);
        switch (siz) {
            case 1:
                output[0]=value;
                break;
            case 3:
                output[0]=0xFD;
                std::copy(value.begin(), value.end()+2,output.begin()+1);
                break;
            case 5:
                output[0]=0xFE;
                std::copy(value.begin(), value.end()+4,output.begin()+1);
                break;
            default:
                output[0]=0xFF;
                std::copy(value.begin(), value.end()+8,output.begin()+1);
                break;

        }

        return output;
    }

    VarInt::operator uint16_t() const {
        return value;
    }

    VarInt::operator uint8_t() const {
        return value;
    }

    VarInt::operator uint32_t() const {
        return value;
    }

    VarInt::operator uint64_t() const {
        return value;
    }

    bool VarInt::operator==(const VarInt &rhs) const {
        return value == rhs.value;
    }

    bool VarInt::operator!=(const VarInt &rhs) const {
        return !(rhs == *this);
    }

    bool VarInt::operator<(const VarInt &rhs) const {
        return value < rhs.value;
    }

    bool VarInt::operator>(const VarInt &rhs) const {
        return rhs < *this;
    }

    bool VarInt::operator<=(const VarInt &rhs) const {
        return !(rhs < *this);
    }

    bool VarInt::operator>=(const VarInt &rhs) const {
        return !(*this < rhs);
    }

    std::ostream &operator<<(std::ostream &os, const VarInt &anInt) {
        os << "value: " << anInt.value;
        return os;
    }


}