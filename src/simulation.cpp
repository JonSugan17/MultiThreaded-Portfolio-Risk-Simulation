#include "simulation.hpp"
#include <random>
#include <algorithm>
#include <omp.h>
#include <stdexcept>
#include <cmath> // Untuk std::isnan dan std::isinf

SimulationResult MonteCarloSimulation::runSimulation(
    const std::vector<double>& means, 
    const std::vector<double>& weights, 
    const std::vector<std::vector<double>>& cholesky,
    int num_simulations, 
    double confidence_level) {
    
    int num_assets = means.size();
    if (num_assets == 0) {
        throw std::invalid_argument("Error: Vektor means kosong.");
    }

    SimulationResult result;
    result.failed_simulations = 0;
    
    std::vector<double> seq_returns(num_simulations);
    std::vector<double> par_returns(num_simulations);

    // ==========================================
    // 1. SEQUENTIAL BASELINE
    // ==========================================
    double start_seq = omp_get_wtime();
    std::mt19937 gen_seq(12345); // Seed statis untuk sequential
    std::normal_distribution<> d_seq(0.0, 1.0);

    for (int iter = 0; iter < num_simulations; ++iter) {
        double port_return = 0.0;
        std::vector<double> Z(num_assets);
        for (int i = 0; i < num_assets; ++i) Z[i] = d_seq(gen_seq);
        
        for (int i = 0; i < num_assets; ++i) {
            double asset_return = means[i];
            for (int j = 0; j <= i; ++j) {
                asset_return += cholesky[i][j] * Z[j];
            }
            port_return += weights[i] * asset_return;
        }
        seq_returns[iter] = port_return;
    }
    double end_seq = omp_get_wtime();
    result.time_sequential = end_seq - start_seq;

    // Mengurutkan dan mencari VaR Sekuensial
    std::sort(seq_returns.begin(), seq_returns.end());
    int var_index = static_cast<int>((1.0 - confidence_level) * num_simulations);
    result.var_sequential = seq_returns[var_index];


    // ==========================================
    // 2. PARALLEL EXECUTION DENGAN RETRY & SYNC
    // ==========================================
    double start_par = omp_get_wtime();
    double total_return = 0.0;
    int local_failed_count = 0; // Digunakan sebagai penampung sementara

    #pragma omp parallel
    {
        std::random_device rd;
        std::mt19937 gen(rd() ^ omp_get_thread_num()); 
        std::normal_distribution<> d(0.0, 1.0); 

        // SINKRONISASI 1: REDUCTION
        // Digunakan untuk mengakumulasi total_return dengan aman tanpa race condition
        #pragma omp for reduction(+:total_return)
        for (int iter = 0; iter < num_simulations; ++iter) {
            double port_return = 0.0;
            bool success = false;
            int retries = 0;
            const int MAX_RETRIES = 3;

            // FAULT TOLERANCE: Retry Logic
            // Jika hasil NaN/Inf (biasanya karena anomali Z), coba ulang
            while (!success && retries < MAX_RETRIES) {
                std::vector<double> Z(num_assets);
                for (int i = 0; i < num_assets; ++i) Z[i] = d(gen);
                
                port_return = 0.0;
                for (int i = 0; i < num_assets; ++i) {
                    double asset_return = means[i];
                    for (int j = 0; j <= i; ++j) {
                        asset_return += cholesky[i][j] * Z[j];
                    }
                    port_return += weights[i] * asset_return;
                }

                // Validasi output angka
                if (std::isnan(port_return) || std::isinf(port_return)) {
                    retries++; // Angka rusak, naikkan hitungan gagal
                } else {
                    success = true; // Berhasil
                }
            }

            // Jika masih gagal setelah maksimal percobaan
            if (!success) {
                port_return = 0.0; // Fallback ke default/netral
                
                // SINKRONISASI 2: ATOMIC
                // Memastikan tidak ada bentrokan saat menambah variabel failed_simulations
                #pragma omp atomic
                result.failed_simulations++; 
            }

            par_returns[iter] = port_return;
            total_return += port_return; // Akumulasi untuk Reduction
        }
    } 
    double end_par = omp_get_wtime();
    result.time_parallel = end_par - start_par;

    // Kalkulasi Rata-rata dari Reduction
    result.average_portfolio_return = total_return / num_simulations;

    // Mengurutkan dan mencari VaR Paralel
    std::sort(par_returns.begin(), par_returns.end());
    result.var_parallel = par_returns[var_index];

    return result;
}