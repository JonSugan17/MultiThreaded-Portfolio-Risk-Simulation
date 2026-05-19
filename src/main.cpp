#include "parser.hpp"
#include "simulation.hpp"
#include <iostream>
#include <iomanip> // Digunakan untuk mengatur presisi desimal pada output

int main() {
    try {
        std::cout << "Memuat data..." << std::endl;
        
        // Membaca file CSV
        auto means = Parser::readVector("data/means.csv");
        auto weights = Parser::readVector("data/weights.csv");
        auto cholesky = Parser::readMatrix("data/cholesky_matrix.csv");

        // Validasi dimensi sebelum lanjut ke simulasi
        if (!Parser::validateDimensions(means, weights, cholesky)) {
            std::cerr << "Proses dihentikan karena validasi dimensi gagal." << std::endl;
            return 1;
        }

        std::cout << "Data berhasil dimuat. Jumlah aset: " << means.size() << std::endl;

        // ==========================================
        // PENGATURAN PARAMETER SIMULASI
        // ==========================================
        int num_simulations = 100000;   // Contoh: 100.000 iterasi
        double confidence_level = 0.95; // Tingkat kepercayaan 95%

        std::cout << "\nMemulai Simulasi Monte Carlo..." << std::endl;
        std::cout << "Jumlah Simulasi  : " << num_simulations << std::endl;
        std::cout << "Confidence Level : " << (confidence_level * 100) << "%" << std::endl;

        // Menjalankan simulasi (Sequential & Parallel)
        SimulationResult result = MonteCarloSimulation::runSimulation(
            means, weights, cholesky, num_simulations, confidence_level
        );

        // ==========================================
        // MENAMPILKAN HASIL SIMULASI
        // ==========================================
        std::cout << "\n================ HASIL SIMULASI ================" << std::endl;
        std::cout << std::fixed << std::setprecision(6);
        
        // Hasil Waktu Eksekusi
        std::cout << "Waktu Eksekusi Sekuensial : " << result.time_sequential << " detik" << std::endl;
        std::cout << "Waktu Eksekusi Paralel    : " << result.time_parallel << " detik" << std::endl;
        
        // Menghitung Speedup (Opsional untuk analisis performa OpenMP)
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
            std::cout << "Status: Semua simulasi berhasil tanpa error numerik." << std::endl;
        }
        std::cout << "================================================" << std::endl;

    } catch (const std::exception& e) {
        // Menangkap error dari std::invalid_argument atau runtime_error dari parser
        std::cerr << "\nFatal Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}