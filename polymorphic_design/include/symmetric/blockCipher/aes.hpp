#ifndef AES_HPP
#define AES_HPP

#include "blockCipher.hpp"
#include "aes.h"

class sisyph::AES: public sisyph::BlockCipher {
public:
    // Default construction
    AES();

    // perfect forwarding constructor. Forwards only block cipher mode
    template<typename cipherMode,
        typename = std::enable_if_t<
            !std::is_base_of<
                sisyph::AES,
                std::decay_t<cipherMode>
            >::value &&
            std::is_constructible<
                std::string,
                std::decay_t<cipherMode>
            >::value
        >
    >
    explicit AES(cipherMode&& blockCipherMode) noexcept:
            BlockCipher(
                "",
                std::forward<cipherMode>(blockCipherMode),
                CryptoPP::AES::MAX_KEYLENGTH,
                CryptoPP::AES::BLOCKSIZE
            ) {
    }

    /*
        perfect forwarding constructor
        It should be straightforward. If it's not,
        refer to template metaprogramming on your search engine
    */
    template<typename pathT, typename cipherMode,
        typename = std::enable_if_t<
            !std::is_base_of<
                sisyph::AES,
                std::decay_t<pathT>
            >::value &&
            std::is_constructible<
                filesystem::path,
                std::decay_t<pathT>
            >::value &&
            !std::is_base_of<
                sisyph::AES,
                std::decay_t<cipherMode>
            >::value &&
            std::is_constructible<
                std::string,
                std::decay_t<cipherMode>
            >::value
        >
    >
    explicit AES(pathT&& fullPath, cipherMode&& blockCipherMode) noexcept:
            BlockCipher(
                std::forward<pathT>(fullPath),
                std::forward<cipherMode>(blockCipherMode),
                CryptoPP::AES::MAX_KEYLENGTH,
                CryptoPP::AES::BLOCKSIZE
            ) {
    }

    // main implementationof encryption
    virtual void encrypt() override;

    // main implementation of decryption
    virtual void decrypt() override;

    // key generation method(key is 256 bit)
    virtual void generateKey() override;

    //initialization vector(IV) generation method
    virtual void generateIV() override;

    // getter for m_encKey. Simply returns encryption key in hex encoded format
    virtual std::string getKey() noexcept override;

    // setter for m_encKey. Sets new ket from hex encoded format
    // Then decodes it to byte format
    virtual void setKey(std::string& newKey) override;
};

#endif
