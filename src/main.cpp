#include "parser.hpp"
#include "simulation.hpp"
#include <iostream>
#include <vector>
#include <iomanip> 
#include <omp.h>

int main() {
    try {
        std::cout << "Memuat data..." << std::endl;
        
        // Membaca file CSV
        auto means = Parser::readVector("data/means.csv");
        auto weights = Parser::readVector("data/weights.csv");
        auto cholesky = Parser::readMatrix("data/cholesky_matrix.csv");

        // Validasi dimensi
        if (!Parser::validateDimensions(means, weights, cholesky)) {
            std::cerr << "Proses dihentikan karena validasi dimensi gagal." << std::endl;
            return 1;
        }

        std::cout << "Data berhasil dimuat. Jumlah aset: " << means.size() << std::endl;

        // ==========================================
        // PENGATURAN PARAMETER SIMULASI
        // ==========================================
        int num_simulations = 100000;   
        double confidence_level = 0.95; 

        std::cout << "\nMemulai Simulasi Monte Carlo..." << std::endl;
        std::cout << "Jumlah Simulasi  : " << num_simulations << std::endl;
        std::cout << "Confidence Level : " << (confidence_level * 100) << "%" << std::endl;

        // Daftar jumlah thread yang akan diuji
        std::vector<int> thread_counts = {1, 2, 4, 8};

        // ==========================================
        // LOOP PENGUJIAN BENCHMARK MULTI-THREAD
        // ==========================================
        double cached_seq_time = 0.0;
        double cached_seq_var = 0.0;

        for (int t : thread_counts) {
            omp_set_num_threads(t); 

            bool is_first_run = (t == 1); // Tandai iterasi pertama untuk caching hasil sekuensial
            
            std::cout << "\n============================================\n";
            std::cout << "        MENGUJI DENGAN " << t << " THREAD        \n";
            std::cout << "============================================\n";

            // Menjalankan simulasi (Masuk ke dalam loop!)
            SimulationResult result = MonteCarloSimulation::runSimulation(
                means, weights, cholesky, num_simulations, confidence_level, is_first_run
            );

            // MANAJEMEN CACHE
            if (is_first_run) {
                // Simpan hasil sekuensial ke cache
                cached_seq_time = result.time_sequential;
                cached_seq_var = result.var_sequential;
            } else {
                // Pakai data dari cache untuk iterasi selanjutnya (jangan hitung ulang)
                result.time_sequential = cached_seq_time;
                result.var_sequential = cached_seq_var;
            }


            std::cout << std::fixed << std::setprecision(6);
            
            // Hasil Waktu Eksekusi
            std::cout << "Waktu Eksekusi Sekuensial : " << result.time_sequential << " detik" << std::endl;
            std::cout << "Waktu Eksekusi Paralel    : " << result.time_parallel << " detik" << std::endl;
            
            // Menghitung Speedup
            double speedup = result.time_sequential / result.time_parallel;
            std::cout << "Speedup Paralel           : " << speedup << "x" << std::endl;
            
            std::cout << "------------------------------------------------" << std::endl;
            
            // Hasil Metrik Keuangan
            std::cout << "Rata-rata Return Portofolio : " << result.average_portfolio_return << std::endl;
            std::cout << "Value at Risk (VaR) Seq     : " << result.var_sequential << std::endl;
            std::cout << "Value at Risk (VaR) Par     : " << result.var_parallel << std::endl;
            
            // Status Fault Tolerance
            std::cout << "------------------------------------------------" << std::endl;
            if (result.failed_simulations > 0) {
                std::cout << "Peringatan: Terdapat " << result.failed_simulations 
                          << " simulasi yang gagal (NaN/Inf) dan diselamatkan." << std::endl;
            } else {
                std::cout << "Status: Komputasi berhasil tanpa error numerik." << std::endl;
            }
        }

        std::cout << "\n================================================" << std::endl;
        std::cout << "STATUS FINAL: SELURUH SKENARIO BENCHMARK SELESAI." << std::endl;
        std::cout << "================================================" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "\nFatal Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}