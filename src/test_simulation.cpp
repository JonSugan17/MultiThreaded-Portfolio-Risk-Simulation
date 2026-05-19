#include "simulation.hpp"
#include <iostream>
#include <vector>
#include <iomanip>

int main() {
    std::cout << "=== Advanced Monte Carlo VaR Simulation ===\n";

    // Dummy Data untuk testing
    std::vector<double> means = {0.0005, 0.0008, 0.0006};
    std::vector<double> weights = {0.4, 0.3, 0.3};
    std::vector<std::vector<double>> cholesky = {
        {0.0150, 0.0000, 0.0000},
        {0.0080, 0.0180, 0.0000},
        {0.0050, 0.0030, 0.0120}
    };

    int num_simulations = 100000;
    double confidence_level = 0.95;

    SimulationResult res = MonteCarloSimulation::runSimulation(
        means, weights, cholesky, num_simulations, confidence_level
    );

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "--- Hasil Sequential ---\n";
    std::cout << "VaR (95%)      : " << res.var_sequential * 100 << "%\n";
    std::cout << "Waktu          : " << res.time_sequential << " detik\n\n";

    std::cout << "--- Hasil Parallel (OpenMP) ---\n";
    std::cout << "VaR (95%)      : " << res.var_parallel * 100 << "%\n";
    std::cout << "Waktu          : " << res.time_parallel << " detik\n";
    std::cout << "Avg Return     : " << res.average_portfolio_return * 100 << "%\n";
    std::cout << "Speedup        : " << res.time_sequential / res.time_parallel << "x lebih cepat\n";
    
    if (res.failed_simulations > 0) {
        std::cout << "[WARNING] Terjadi " << res.failed_simulations << " iterasi gagal yang di-fallback.\n";
    } else {
        std::cout << "[OK] Seluruh iterasi berhasil divalidasi.\n";
    }

    return 0;
}