#include "twofish.hpp"

// Default constructed mode,
sisyph::Twofish::Twofish():
                BlockCipher(
                    "",
                    "CBC",
                    CryptoPP::Twofish::MAX_KEYLENGTH,
                    CryptoPP::Twofish::BLOCKSIZE
                ) {
}

// Decryption implementation
void sisyph::Twofish::encrypt() {
    /* if extenstion of the processing file is .sisyph
     skip it, cause we that means that it is encrypted.
     We don't want to encrypt file twice
    */
    if (m_currentPath.extension() == ".sisyph") {
        std::cerr << "Already encrypted... Skipping --------> "
                  << m_currentPath.generic_string() << std::endl;
        return;
    }

    /*  process file according to the Decryption template type parameter.
        Decryption template type parameter includes
        type of algorithm, type of operation, and type of
        block cipher mode of operation, which means that we can
        process a file in any way we want

        if block cipher mode is CBC, process it with CBC cipher mode
    */
    if(m_blockCipherModes["CBC"])
        process<CBC_Mode<CryptoPP::Twofish>::Encryption>(true);
    // if m_blockCipherMode is ECB, process it with ECB cipher mode
    // ECB is not recommended. Supported just optionally
    else if(m_blockCipherModes["ECB"])
        process<ECB_Mode<CryptoPP::Twofish>::Encryption>(true);
    // if m_blockCipherMode is CTR, process it with CTR cipher mode
    else if(m_blockCipherModes["CTR"])
        process<CTR_Mode<CryptoPP::Twofish>::Encryption>(true);
    // Following are authenticated encryption schemes
    // if m_blockCipherMode is GCM, process it with GCM cipher mode
    else if(m_blockCipherModes["GCM"])
        processAuthentic<GCM<CryptoPP::Twofish>::Encryption>(true);
//    // if m_blockCipherMode is CCM, process it with CCM cipher mode
//    else if(m_blockCipherMode == "CCM")
//        process<CCM<CryptoPP::Twofish>::Encryption>(".sisyph");
//    // if m_blockCipherMode is EAX, process it with EAX cipher mode
    else if(m_blockCipherModes["EAX"])
        processAuthentic<EAX<CryptoPP::Twofish>::Encryption>(true);

    else {
        std::cerr << "Block cipher mode doesn't exist\n";
        exit(-1);
    }
}

// decryption implementation
void sisyph::Twofish::decrypt() {
    /* if extenstion of the processing file is not .sisyph
     skip it, cause we that means that it is decrypted.
     We don't want to decrypt file twice
    */
    if (m_currentPath.extension() != ".sisyph") {
        std::cerr << "Already decrypted... Skipping -------> "
                  << m_currentPath.generic_string() << std::endl;
        return;
    }

    /* process file. Passing decryption template type parameter,
       just like above.
       We can even choose block cipher modes, that will be implemented soon.
    */
    if(m_blockCipherModes["CBC"])
        process<CBC_Mode<CryptoPP::Twofish>::Decryption>(false);
    // if m_blockCipherMode is ECB, process it with ECB cipher mode
    else if(m_blockCipherModes["ECB"])
        process<ECB_Mode<CryptoPP::Twofish>::Decryption>(false);
    // if m_blockCipherMode is CTR, process it with CTR cipher mode
    else if(m_blockCipherModes["CTR"])
        process<CTR_Mode<CryptoPP::Twofish>::Decryption>(false);
    // Following are authenticated Decryption schemes
    // if m_blockCipherMode is GCM, process it with GCM cipher mode
    else if(m_blockCipherModes["GCM"])
        processAuthentic<GCM<CryptoPP::Twofish>::Decryption>(false);
//    // if m_blockCipherMode is CCM, process it with CCM cipher mode
//    else if(m_blockCipherMode == "CCM")
//        process<CCM<CryptoPP::Twofish>::Decryption>(".sisyph");
    // if m_blockCipherMode is EAX, process it with EAX cipher mode
    else if(m_blockCipherModes["EAX"])
        processAuthentic<EAX<CryptoPP::Twofish>::Decryption>(false);

    else {
        std::cerr <<"Block cipher mode doesn't exist\n";
        exit(-1);
    }
}

// Decryption key generation. Both hex encoded and decoded(in bytes);
void sisyph::Twofish::generateKey() {
    // Generate Decryption key
    generate(m_byteKey, m_byteKey.size(), m_encKey);

    // Now we've gotta generate initialization vector(IV), it's important
    // if we're processing data in ECB mode, then we don't need IV
    if(!m_blockCipherModes["ECB"])
        generateIV();
}

// initialization vector(IV) generation. Both hex encoded and decoded(in bytes);
void sisyph::Twofish::generateIV() {
    // Generate initialization vector(IV)
    generate(m_byteIV, m_byteIV.size(), m_encIV);
}

/* Get the final key representation. It consists of hex encoded key
   appended with hex encoded IV
*/
std::string sisyph::Twofish::getKey() noexcept {
    // ECB mode doesn't use IV, so we don't need it if ECB mode enabled
    if(m_blockCipherModes["ECB"])
        return m_encKey;
    return m_encKey + m_encIV;
}

/* sets Decryption key and IV. newKey is hex encoded key appended with hex
   encoded IV, which represent the overall key, or, newKey is just a filename
   with sequence of concatenated Decryption key and IV
*/
void sisyph::Twofish::setKey(std::string& newKey) {
    // process new key. Set both, key and IV.
    processKey(newKey);
}
