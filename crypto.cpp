//
// Created by edward yue on 2020/11/3.
//

#include "crypto.h"
#include <cryptopp/rsa.h>
#include <cryptopp/pem.h>
#include <cryptopp/pssr.h>
#include <cryptopp/sha.h>
#include <iostream>
#include <assert.h>
CryptoPP::AutoSeededRandomPool Crypto::s_rng_;

void Crypto::Base64Encode(const std::string &str, std::string *out) {
    assert(out);
    CryptoPP::Base64Encoder encoder(nullptr, false);
    encoder.Put((CryptoPP::byte*)(str.data()), str.size());
    encoder.MessageEnd();
    CryptoPP::word64 size = encoder.MaxRetrievable();
    if(size){
        out->resize(size);
        encoder.Get((CryptoPP::byte*)&(*out)[0], out->size());
    }
}
bool Crypto::GenerateKeyPair(std::string *private_key, std::string *public_key) {
    assert(private_key);
    assert(public_key);

    CryptoPP::RSA::PrivateKey privateKey;
    privateKey.GenerateRandomWithKeySize(s_rng_, 2048);
    CryptoPP::RSA::PublicKey publicKey(privateKey);

    CryptoPP::PEM_Save(CryptoPP::StringSink(*private_key).Ref(), privateKey);
    CryptoPP::PEM_Save(CryptoPP::StringSink(*public_key).Ref(), publicKey);
    return true;
}
bool Crypto::Sign(const std::string &private_key, const std::string &value, std::string *signature) {
    try{
        // get private key
        CryptoPP::RSA::PrivateKey privateKey;
        CryptoPP::StringSource privateKeySrc(private_key, true);
        CryptoPP::PEM_Load(privateKeySrc, privateKey);

        // sign sha256 digest
        CryptoPP::RSASS<CryptoPP::PSS, CryptoPP::SHA256>::Signer signer(privateKey);
        size_t length = signer.MaxSignatureLength();
        CryptoPP::SecByteBlock sign_block(length);
        length = signer.SignMessage(s_rng_, (CryptoPP::byte*)value.data(), value.size(), sign_block);
        sign_block.resize(length);

        //base64 encode
        CryptoPP::Base64Encoder encoder;
        encoder.Put(sign_block.data(), sign_block.size());
        encoder.MessageEnd();
        CryptoPP::word64 size = encoder.MaxRetrievable();
        std::string encoded;

        if(size){
            encoded.resize(size);
            encoder.Get((CryptoPP::byte*)&encoded[0], encoded.size());
            *signature = encoded;
            return true;
        }else{
            std::cout << "base64 encode message error\n";
            return false;
        }
    } catch (CryptoPP::Exception &ex) {
        std::cout << ex.GetWhat();
    }
    catch (...) {
        std::cout << "Unknown error\n";
    }
    return false;
}

