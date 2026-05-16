#include <gtest/gtest.h>
#include "simulation.hpp"
#include <omp.h>
#include <vector>
#include <cmath>

class MonteCarloSimulationTest : public ::testing::Test {
protected:
    std::vector<double> means;
    std::vector<double> weights;
    std::vector<std::vector<double>> cholesky;
    double confidence_level;
    int num_simulations;

    void SetUp() override {
        // Setup data portofolio standar (2 Aset) untuk pengujian umum
        means = {0.05, 0.08};
        weights = {0.5, 0.5};
        cholesky = {
            {0.10, 0.00},
            {0.03, 0.12}
        };
        confidence_level = 0.95;
        num_simulations = 10000; // Jumlah sampel yang cukup untuk konvergensi statistika
    }
};

// =========================================================================
// 1. TEST: Memastikan VaR Paralel vs VaR Sekuensial (Pendekatan Statistika)
// =========================================================================
TEST_F(MonteCarloSimulationTest, CompareParallelAndSequentialVaR) {
    MonteCarloSimulation sim;
    SimulationResult result = sim.runSimulation(means, weights, cholesky, num_simulations, confidence_level);
    
    // Catatan: Karena generator paralel menggunakan std::random_device (non-deterministik),
    // nilai tidak akan sama persis bita-per-bita (==), melainkan harus mendekati secara statistika.
    // Kita gunakan batas toleransi (epsilon) sebesar 0.05.
    double epsilon = 0.05;
    EXPECT_NEAR(result.var_sequential, result.var_parallel, epsilon);
}

// =========================================================================
// 2. EDGE CASE 1: Menguji jika input jumlah aset kosong (Exception Handling)
// =========================================================================
TEST_F(MonteCarloSimulationTest, EmptyAssetsThrowsArgumentException) {
    std::vector<double> empty_means;
    std::vector<double> empty_weights;
    std::vector<std::vector<double>> empty_cholesky;
    
    MonteCarloSimulation sim;
    
    // Memastikan melempar std::invalid_argument saat melempar vektor kosong
    EXPECT_THROW({
        sim.runSimulation(empty_means, empty_weights, empty_cholesky, num_simulations, confidence_level);
    }, std::invalid_argument);
}

// =========================================================================
// 3. EDGE CASE 2: Menguji program berjalan lancar dengan hanya 1 thread (N=1)
// =========================================================================
TEST_F(MonteCarloSimulationTest, RunSmoothlyWithSingleThread) {
    MonteCarloSimulation sim;
    
    // Paksa OpenMP hanya menggunakan 1 thread
    omp_set_num_threads(1);
    
    SimulationResult result;
    EXPECT_NO_THROW({
        result = sim.runSimulation(means, weights, cholesky, 1000, confidence_level);
    });
    
    // Pastikan tidak ada kegagalan simulasi dan hasil valid angka
    EXPECT_EQ(result.failed_simulations, 0);
    EXPECT_FALSE(std::isnan(result.var_parallel));
    EXPECT_FALSE(std::isinf(result.var_parallel));
}

// =========================================================================
// 4. EDGE CASE 3: Menguji stabilitas saat jumlah simulasi diatur sangat besar
// =========================================================================
TEST_F(MonteCarloSimulationTest, LargeSimulationStability) {
    MonteCarloSimulation sim;
    int large_num_simulations = 1000000; // 1 Juta Simulasi
    
    SimulationResult result;
    EXPECT_NO_THROW({
        result = sim.runSimulation(means, weights, cholesky, large_num_simulations, confidence_level);
    });
    
    // Memastikan kestabilan memori & kalkulasi data besar
    EXPECT_EQ(result.failed_simulations, 0);
    EXPECT_GT(result.time_parallel, 0.0);
    EXPECT_FALSE(std::isnan(result.average_portfolio_return));
}

// =========================================================================
// 5. TEST DETERMINISM: Konsistensi hasil sekuensial dengan seed statis
// =========================================================================
TEST_F(MonteCarloSimulationTest, SequentialDeterminismWithStaticSeed) {
    MonteCarloSimulation sim;
    
    // Jalankan simulasi pertama
    SimulationResult run_1 = sim.runSimulation(means, weights, cholesky, 5000, confidence_level);
    
    // Jalankan simulasi kedua
    SimulationResult run_2 = sim.runSimulation(means, weights, cholesky, 5000, confidence_level);
    
    // Karena bagian sekuensial menggunakan seed statis (12345), 
    // hasilnya HARUS konsisten sama persis antara run 1 dan run 2.
    EXPECT_DOUBLE_EQ(run_1.var_sequential, run_2.var_sequential);
}