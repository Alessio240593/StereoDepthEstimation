#include <CrossCorrelation.hpp>
#include <iostream>
#include <vector>
#include <cstring>

/// Dimensione del kernel
#define KERNEL_SIZE 3
/// Larghezza della matrice sorgente
#define WIDTH 10

int main()
{
    const std::vector<uint8_t> src1{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    const std::vector<uint8_t> src2{1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
                          1, 1, 1, 0, 0, 0, 1, 0, 0, 0,
                          1, 1, 1, 1, 1, 1, 0, 0, 0, 0};

    /*std::vector<uint8_t> k{1, 1, 1,
                       1, 1, 1,
                       1, 1, 1};*/

    std::vector<uint8_t> dst(WIDTH - (KERNEL_SIZE - 1), 0);
    std::vector<uint8_t> dst2(KERNEL_SIZE + 2 + WIDTH + 2, 0);

    /*for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 10; j++) {
            std::cout << src.at((i * 10) + j) << " ";
        }
        std::cout <<"\n";
    }*/

    argMaxCorrVector<uint8_t>(src1.data(), src2.data(), dst.data(), KERNEL_SIZE, WIDTH);

    uint8_t *tmp =dst.data();
    
    for (size_t j = 0; j < 8; j++) {
        std::cout << +*(tmp + j) << " ";
    }
    std::cout <<"\n\n";

    padding<uint8_t>(src2.data(), dst2.data(), 3, 10, 5, 12, 3, 3);
    uint8_t *tmp2 = dst2.data(); 

    for (size_t i = 0; i < 5; i++) {
        for (size_t j = 0; j < 12; j++) {
            std::cout << +*(tmp2 +(i * 12) + j) << " ";
        }
        std::cout <<"\n";
    }

    exit(EXIT_SUCCESS);
}