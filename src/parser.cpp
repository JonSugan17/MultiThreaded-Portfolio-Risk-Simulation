#include "parser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

// Implementasi fungsi-fungsi dalam kelas Parser
std::vector<double> Parser::readVector(const std::string& filename, bool skipHeader) {
    std::vector<double> result;
    std::ifstream file(filename);

    // Validasi pembukaan file
    if (!file.is_open()) {
        throw std::runtime_error("Gagal membuka file: " + filename);
    }

    // Membaca setiap baris dan mengonversi ke double
    std::string line;
    if (skipHeader) std::getline(file, line); // Lewati header

    // Membaca data baris per baris
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        try {
            result.push_back(std::stod(line));
        } catch (...) {
            // Menangani kasus jika ada baris non-angka
            std::cerr << "Peringatan: Baris tidak valid dalam file " << filename << std::endl;
            continue;
        }
    }
    file.close();
    return result;
}

// Implementasi fungsi untuk membaca matriks dari file CSV
std::vector<std::vector<double>> Parser::readMatrix(const std::string& filename) {
    std::vector<std::vector<double>> matrix;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Gagal membuka file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::vector<double> row;
        std::stringstream ss(line);
        std::string value;

        while (std::getline(ss, value, ',')) {
            try {
                row.push_back(std::stod(value));
            } catch (...) {
                std::cerr << "Peringatan: Nilai tidak valid dalam file " << filename << std::endl;
                continue;
            }
        }
        matrix.push_back(row);
    }
    file.close();
    return matrix;
}

// Implementasi fungsi untuk memvalidasi dimensi data
bool Parser::validateDimensions(const std::vector<double>& means, 
                               const std::vector<double>& weights, 
                               const std::vector<std::vector<double>>& cholesky) {
    size_t n = means.size();
    
    if (weights.size() != n) {
        std::cerr << "Error: Ukuran weights tidak sama dengan means!" << std::endl;
        return false;
    }
    
    if (cholesky.size() != n) {
        std::cerr << "Error: Jumlah baris matriks Cholesky tidak sesuai!" << std::endl;
        return false;
    }

    for (const auto& row : cholesky) {
        if (row.size() != n) {
            std::cerr << "Error: Matriks Cholesky harus berbentuk persegi (N x N)!" << std::endl;
            return false;
        }
    }

    return true;
}