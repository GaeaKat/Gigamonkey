// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "gtest/gtest.h"
#include <gigamonkey/p2p/node.hpp>

#include <gmock/gmock.h>
#include <gigamonkey/p2p/var_string.hpp>



namespace Gigamonkey::p2p {
    TEST(VarString,VarStringToBytes) {
        VarString varString("testString");
        data::bytes output=(data::bytes)varString;
        EXPECT_EQ(output[0],0xA);
        EXPECT_EQ(output.size(),11);
    }

    TEST(VarString,VarStringFromBytes) {
        data::bytes input=data::bytes(6);
        input[0]=0x05;
        input[1]=0x54;
        input[2]=0x65;
        input[3]=0x73;
        input[4]=0x74;
        input[5]=0x54;
        VarString str(input);
        EXPECT_EQ((std::string)str,"TestT");

        VarString strr("");
        std::cout << (data::bytes)strr << std::endl;
        std::cout << strr << std::endl;
        std::cout << str << std::endl;
    }
}