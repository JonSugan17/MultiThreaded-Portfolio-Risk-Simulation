#include "simulation.hpp"
#include <random>
#include <algorithm>
#include <omp.h>
#include <stdexcept>

double MonteCarloSimulation::calculateVaR(const std::vector<double>& means, 
                                          const std::vector<double>& weights, 
                                          const std::vector<std::vector<double>>& cholesky,
                                          int num_simulations, 
                                          double confidence_level) {
    
    int num_assets = means.size();
    if (num_assets == 0) {
        throw std::invalid_argument("Error: Vektor means kosong. Tidak ada aset untuk disimulasikan.");
    }

    // Vektor untuk menyimpan hasil simulasi return portofolio
    std::vector<double> portfolio_returns(num_simulations);

    // Blok Paralel OpenMP
    #pragma omp parallel
    {
        // Setiap thread mendapatkan Random Number Generator-nya sendiri
        std::random_device rd;
        std::mt19937 gen(rd() ^ omp_get_thread_num()); 
        std::normal_distribution<> d(0.0, 1.0); 

        #pragma omp for
        for (int iter = 0; iter < num_simulations; ++iter) {
            std::vector<double> Z(num_assets);
            
            for (int i = 0; i < num_assets; ++i) {
                Z[i] = d(gen);
            }

            double port_return = 0.0;
            
            // Menghitung return portofolio
            for (int i = 0; i < num_assets; ++i) {
                double asset_return = means[i];
                for (int j = 0; j <= i; ++j) { // Hanya loop matriks segitiga bawah
                    asset_return += cholesky[i][j] * Z[j];
                }
                port_return += weights[i] * asset_return;
            }
            
            portfolio_returns[iter] = port_return;
        }
    } // Akhir blok parallel

    // Mengurutkan hasil dari terburuk ke terbaik
    std::sort(portfolio_returns.begin(), portfolio_returns.end());
    
    // Mencari indeks VaR berdasarkan tingkat kepercayaan
    int var_index = static_cast<int>((1.0 - confidence_level) * num_simulations);
    
    // Mengembalikan nilai kerugian (Value at Risk)
    return portfolio_returns[var_index];
}
