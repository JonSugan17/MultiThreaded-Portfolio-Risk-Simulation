#include "parser.hpp"
#include <iostream>

int main() {
    try {
        auto means = Parser::readVector("data/means.csv");
        auto weights = Parser::readVector("data/weights.csv");
        auto cholesky = Parser::readMatrix("data/cholesky_matrix.csv");

        if (Parser::validateDimensions(means, weights, cholesky)) {
            std::cout << "Data berhasil dimuat. Jumlah aset: " << means.size() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
