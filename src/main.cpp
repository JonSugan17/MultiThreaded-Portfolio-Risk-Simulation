#include "parser.hpp"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <omp.h>
#include <iomanip>

int main() {
    try {
        // 1. Memuat data menggunakan parser yang sudah dibuat
        auto means = Parser::readVector("data/means.csv");
        auto weights = Parser::readVector("data/weights.csv");
        auto cholesky = Parser::readMatrix("data/cholesky_matrix.csv");

        // Validasi dimensi data
        if (!Parser::validateDimensions(means, weights, cholesky)) {
            return 1;
        }

        int num_assets = means.size();
        
        // Parameter simulasi
        const int num_simulations = 1000000; // 1 Juta iterasi simulasi
        const double confidence_level = 0.95; // Tingkat kepercayaan 95%

        std::cout << "========================================\n";
        std::cout << "  Simulasi Monte Carlo VaR (OpenMP)\n";
        std::cout << "========================================\n";
        std::cout << "Jumlah aset      : " << num_assets << "\n";
        std::cout << "Jumlah iterasi   : " << num_simulations << "\n";
        std::cout << "Tingkat konfidens: " << (confidence_level * 100) << "%\n";
        std::cout << "Memulai komputasi paralel...\n";

        // Vektor untuk menyimpan hasil simulasi return portofolio
        std::vector<double> portfolio_returns(num_simulations);

        // Memulai perhitungan waktu
        double start_time = omp_get_wtime();

        // 2. Blok Paralel OpenMP
        #pragma omp parallel
        {
            // PENTING: Setiap thread memerlukan Random Number Generator (RNG) sendiri
            // untuk menghindari race condition dan memastikan angka acak yang independen.
            std::random_device rd;
            
            // Menggabungkan random device dengan nomor thread sebagai seed
            std::mt19937 gen(rd() ^ omp_get_thread_num()); 
            std::normal_distribution<> d(0.0, 1.0); // Distribusi Normal Standar (Mean=0, StdDev=1)

            // Membagi iterasi loop ke berbagai thread
            #pragma omp for
            for (int iter = 0; iter < num_simulations; ++iter) {
                std::vector<double> Z(num_assets);
                
                // Menghasilkan variabel acak normal standar Z untuk setiap aset
                for (int i = 0; i < num_assets; ++i) {
                    Z[i] = d(gen);
                }

                double port_return = 0.0;
                
                // Menghitung return berkorelasi (R = Mean + L * Z)
                for (int i = 0; i < num_assets; ++i) {
                    double asset_return = means[i];
                    
                    // L adalah matriks segitiga bawah (Cholesky Lower Triangular)
                    // Jadi kita hanya perlu mengalikan hingga indeks j <= i
                    for (int j = 0; j <= i; ++j) {
                        asset_return += cholesky[i][j] * Z[j];
                    }
                    
                    // Menambahkan ke total return portofolio berdasarkan bobot
                    port_return += weights[i] * asset_return;
                }
                
                // Menyimpan hasil untuk simulasi ini
                portfolio_returns[iter] = port_return;
            }
        }

        // Mengakhiri perhitungan waktu eksekusi OpenMP
        double end_time = omp_get_wtime();

        // 3. Menghitung Value at Risk (VaR)
        // Mengurutkan return dari yang terburuk (paling negatif) ke yang terbaik
        std::sort(portfolio_returns.begin(), portfolio_returns.end());
        
        // Mencari indeks berdasarkan tingkat kepercayaan (persentil ke-5 untuk CL 95%)
        int var_index = static_cast<int>((1.0 - confidence_level) * num_simulations);
        
        // Nilai VaR adalah return maksimum kerugian pada persentil tersebut
        double var_value = portfolio_returns[var_index];

        // 4. Menampilkan Hasil
        std::cout << "========================================\n";
        std::cout << "Waktu eksekusi   : " << std::fixed << std::setprecision(4) 
                  << (end_time - start_time) << " detik\n";
        
        // Output persentase VaR
        std::cout << "1-Day VaR (95%)  : " << std::fixed << std::setprecision(6) 
                  << var_value * 100 << " % dari total portofolio\n";
        std::cout << "========================================\n";

    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
