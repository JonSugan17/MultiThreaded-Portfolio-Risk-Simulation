#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>

// 1. Tambahkan struktur data ini untuk menampung hasil
struct SimulationResult {
    double var_sequential;
    double var_parallel;
    double time_sequential;
    double time_parallel;
    double average_portfolio_return;
    int failed_simulations;
};

class MonteCarloSimulation {
public:
    // 2. Pastikan nama fungsinya adalah runSimulation (bukan calculateVaR)
    static SimulationResult runSimulation(
        const std::vector<double>& means, 
        const std::vector<double>& weights, 
        const std::vector<std::vector<double>>& cholesky,
        int num_simulations, 
        double confidence_level,
        bool run_sequential = true);
};

#endif