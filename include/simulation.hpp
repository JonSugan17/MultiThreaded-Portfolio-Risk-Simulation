#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>
struct SimulationResult {
    double var_parallel;
    double var_sequential;
    double time_parallel;
    double time_sequential;
    double average_portfolio_return;
    int failed_simulations;
}
    


class MonteCarloSimulation {
public:
    /**
     * Menghitung Value at Risk (VaR) menggunakan simulasi Monte Carlo terparalelisasi (OpenMP)
     * @param means Vektor rata-rata return aset
     * @param weights Vektor bobot portofolio
     * @param cholesky Matriks dekomposisi Cholesky dari matriks korelasi
     * @param num_simulations Jumlah iterasi simulasi yang ingin dijalankan
     * @param confidence_level Tingkat kepercayaan (misalnya: 0.95 untuk 95%)
     * @return Nilai persentase VaR kerugian
     */
    static double calculateVaR(const std::vector<double>& means, 
                               const std::vector<double>& weights, 
                               const std::vector<std::vector<double>>& cholesky,
                               int num_simulations, 
                               double confidence_level);
};

#endif
