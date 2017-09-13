// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.
#include <algsxmss.h>
#include <xmss.h>
#include <vector>
#include <iostream>
#include "gtest/gtest.h"
#include <misc.h>

namespace {
#define XMSS_HEIGHT 5

    TEST(XMSS, Instantiation) {
        std::vector<unsigned char> seed(48, 0);

        Xmss xmss(seed, XMSS_HEIGHT);

        auto pk = xmss.getPK();
        auto sk = xmss.getSK();

        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "seed:" << seed.size() << " bytes\n" << bin2hstr(seed, 48) << std::endl;
        std::cout << "pk  :" << pk.size() << " bytes\n" << bin2hstr(pk, 48) << std::endl;
        std::cout << "sk  :" << sk.size() << " bytes\n" << bin2hstr(sk, 48) << std::endl;

        EXPECT_EQ(seed, xmss.getSeed());
    }

    TEST(XMSS, SignatureLen) {
        std::vector<unsigned char> seed(48, 0);

        Xmss xmss4(seed, 4);
        EXPECT_EQ(2308, xmss4.getSignatureSize());

        Xmss xmss6(seed, 6);
        EXPECT_EQ(2372, xmss6.getSignatureSize());
    }

    TEST(XMSS, Sign) {
        std::vector<unsigned char> seed(48, 0);

        Xmss xmss(seed, XMSS_HEIGHT);

        std::string message = "This is a test message";
        std::vector<unsigned char> data(message.begin(), message.end());

        auto signature = xmss.sign(data);

        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "data       :" << data.size() << " bytes\n" << bin2hstr(data, 64) << std::endl;
        std::cout << "signature  :" << signature.size() << " bytes\n" << bin2hstr(signature, 64) << std::endl;
    }


    TEST(XMSS, Verify) {
        std::vector<unsigned char> seed(48, 0);

        Xmss xmss(seed, XMSS_HEIGHT);

        std::string message = "This is a test message";
        std::vector<unsigned char> data_ref(message.begin(), message.end());
        std::vector<unsigned char> data(message.begin(), message.end());

        auto pk = xmss.getPK();
        auto sk = xmss.getSK();
        std::cout << std::endl;
        std::cout << "seed:" << seed.size() << " bytes\n" << bin2hstr(seed, 32) << std::endl;
        std::cout << "pk  :" << pk.size() << " bytes\n" << bin2hstr(pk, 32) << std::endl;
        std::cout << "sk  :" << sk.size() << " bytes\n" << bin2hstr(sk, 32) << std::endl;

        auto signature = xmss.sign(data);

        EXPECT_EQ(data, data_ref);

        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "data       :" << data.size() << " bytes\n" << bin2hstr(data, 64) << std::endl;
        std::cout << "signature  :" << signature.size() << " bytes\n" << bin2hstr(signature, 64) << std::endl;

        EXPECT_TRUE(Xmss::verify(data, signature, pk, XMSS_HEIGHT));

        signature[1] += 1;
        EXPECT_FALSE(Xmss::verify(data, signature, xmss.getPK(), XMSS_HEIGHT));
    }
}
