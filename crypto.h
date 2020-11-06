//
// Created by edward yue on 2020/11/3.
//

#ifndef KR_CRYPTO_H
#define KR_CRYPTO_H
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>
class Crypto {
public:
    static CryptoPP::AutoSeededRandomPool s_rng_;
    static void Base64Encode(const std::string &str, std::string *out);
    static bool GenerateKeyPair(std::string *private_key, std::string *public_key);
    static bool Sign(const std::string& private_key, const std::string& value, std::string *signature);
};


#endif //KR_CRYPTO_H
