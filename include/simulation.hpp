#include "parser.hpp"
#include "simulation.hpp"
#include <iostream>
#include <iomanip>

int main() {
    try {
        std::cout << "Memuat data dari direktori data/...\n";
        
        // 1. Parse Data CSV
        auto means = Parser::readVector("data/means.csv");
        auto weights = Parser::readVector("data/weights.csv");
        auto cholesky = Parser::readMatrix("data/cholesky_matrix.csv");

        // Validasi Dimensi
        if (!Parser::validateDimensions(means, weights, cholesky)) {
            std::cerr << "Validasi data gagal. Menghentikan program.\n";
            return 1;
        }

        int num_assets = means.size();
        int num_simulations = 1000000; // 1 Juta iterasi
        double confidence_level = 0.95;

        std::cout << "Data berhasil dimuat. Jumlah aset: " << num_assets << "\n";
        std::cout << "=================================================\n";
        std::cout << " Memulai Simulasi Monte Carlo VaR (C++ & OpenMP) \n";
        std::cout << "=================================================\n";

        // 2. Jalankan Simulasi Terpadu (Sekuensial & Paralel)
        SimulationResult res = MonteCarloSimulation::runSimulation(
            means, weights, cholesky, num_simulations, confidence_level
        );

        // 3. Tampilkan Hasil Komparasi
        std::cout << std::fixed << std::setprecision(6);
        
        std::cout << "[ HASIL SEQUENTIAL BASELINE ]\n";
        std::cout << "1-Day VaR (95%)  : " << res.var_sequential * 100 << "%\n";
        std::cout << "Waktu Eksekusi   : " << res.time_sequential << " detik\n\n";

        std::cout << "[ HASIL PARALLEL (OpenMP) ]\n";
        std::cout << "1-Day VaR (95%)  : " << res.var_parallel * 100 << "%\n";
        std::cout << "Waktu Eksekusi   : " << res.time_parallel << " detik\n";
        std::cout << "Avg Port. Return : " << res.average_portfolio_return * 100 << "%\n";
        
        std::cout << "\n[ KESIMPULAN METRIK ]\n";
        std::cout << "Speedup          : " << std::setprecision(2) 
                  << (res.time_sequential / res.time_parallel) << "x lebih cepat\n";
        
        if (res.failed_simulations > 0) {
            std::cout << "Fault Tolerance  : " << res.failed_simulations << " simulasi dinetralkan akibat error angka acak (NaN/Inf).\n";
        } else {
            std::cout << "Fault Tolerance  : Aman. 0 error komputasi terdeteksi selama iterasi.\n";
        }
        std::cout << "=================================================\n";

    } catch (const std::exception& e) {
        std::cerr << "Fatal Error saat runtime: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}