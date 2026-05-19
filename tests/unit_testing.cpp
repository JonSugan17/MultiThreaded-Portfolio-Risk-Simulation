#include "../include/simulation.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <string>

#define TEST_CONTEXT(test_name) std::cout << "[RUN] " << test_name << "...\n"
#define TEST_PASS() std::cout << "\033[32m  -> STATUS: PASS\033[0m\n\n"
#define TEST_FAIL(reason) { std::cout << "\033[31m  -> STATUS: FAIL (" << reason << ")\033[0m\n\n"; total_failed++; }

int total_failed = 0;

void setup_dummy_data(std::vector<double>& means, std::vector<double>& weights, std::vector<std::vector<double>>& cholesky) {
    means = {0.0005, 0.0003};
    weights = {0.5, 0.5};
    cholesky = {
        {1.0, 0.0},
        {0.45, 0.893}
    };
}

// ----------------==================================----------------
// 1. SMOKE TEST: Memastikan fungsi berjalan tanpa crash
// ----------------==================================----------------
void test_smoke() {
    TEST_CONTEXT("Test 1: Smoke Test");
    std::vector<double> means, weights;
    std::vector<std::vector<double>> cholesky;
    setup_dummy_data(means, weights, cholesky);

    try {
        SimulationResult res = MonteCarloSimulation::runSimulation(means, weights, cholesky, 1000, 0.95);
        TEST_PASS();
    } catch (const std::exception& e) {
        TEST_FAIL(std::string("Crash terdeteksi: ") + e.what());
    }
}

// ----------------==================================----------------
// 2. CORRECTNESS TEST: Cek keselarasan paralel vs sekuensial
// ----------------==================================----------------
void test_correctness() {
    TEST_CONTEXT("Test 2: Correctness Test (Parallel == Sequential)");
    std::vector<double> means, weights;
    std::vector<std::vector<double>> cholesky;
    setup_dummy_data(means, weights, cholesky);

    try {
        SimulationResult res = MonteCarloSimulation::runSimulation(means, weights, cholesky, 50000, 0.95);

        // Toleransi perbedaan hasil acak harian
        double delta = std::abs(res.var_parallel - res.var_sequential);
        
        if (delta < 0.01) {
            TEST_PASS();
        } else {
            TEST_FAIL("Selisih hasil paralel dan sekuensial terlalu jauh. Delta: " + std::to_string(delta));
        }
    } catch (const std::exception& e) {
        TEST_FAIL(std::string("Error eksekusi: ") + e.what());
    }
}

// ----------------==================================----------------
// 3. EDGE CASE TEST: Input Kosong (N = 0 Aset)
// ----------------==================================----------------
void test_empty_input() {
    TEST_CONTEXT("Test 3: Edge Case (Empty Input / Zero Assets)");
    std::vector<double> empty_means;
    std::vector<double> empty_weights;
    std::vector<std::vector<double>> empty_cholesky;

    try {
        MonteCarloSimulation::runSimulation(empty_means, empty_weights, empty_cholesky, 1000, 0.95);
        TEST_FAIL("Program tidak melempar exception saat diberikan input kosong.");
    } catch (const std::invalid_argument& e) {
        TEST_PASS(); // Ekspektasi terpenuhi
    } catch (...) {
        TEST_FAIL("Melemparkan tipe exception yang salah (bukan invalid_argument).");
    }
}

// ----------------==================================----------------
// 4. EDGE CASE TEST: N = 1 (Simulasi Tunggal)
// ----------------==================================----------------
void test_single_simulation() {
    TEST_CONTEXT("Test 4: Edge Case (N = 1 Simulation)");
    std::vector<double> means, weights;
    std::vector<std::vector<double>> cholesky;
    setup_dummy_data(means, weights, cholesky);

    try {
        SimulationResult res = MonteCarloSimulation::runSimulation(means, weights, cholesky, 1, 0.95);
        
        if (!std::isnan(res.var_parallel) && !std::isinf(res.var_parallel)) {
            TEST_PASS();
        } else {
            TEST_FAIL("Hasil komputasi paralel menghasilkan nilai NaN atau Infinity.");
        }
    } catch (const std::exception& e) {
        TEST_FAIL(std::string("Gagal mengeksekusi N=1: ") + e.what());
    }
}

// ----------------==================================----------------
// 5. STRESS TEST & FAULT TOLERANCE CHECK
// ----------------==================================----------------
void test_large_simulation_and_faults() {
    TEST_CONTEXT("Test 5: Stress Test (N = 1 Juta) & Fault Tolerance");
    std::vector<double> means, weights;
    std::vector<std::vector<double>> cholesky;
    setup_dummy_data(means, weights, cholesky);

    try {
        SimulationResult res = MonteCarloSimulation::runSimulation(means, weights, cholesky, 1000000, 0.95);
        
        if (res.var_parallel != 0.0) {
            std::cout << "      [INFO] Waktu Eksekusi: " << res.time_parallel << "s\n";
            std::cout << "      [INFO] Simulasi Gagal/Fallback: " << res.failed_simulations << "\n";
            TEST_PASS();
        } else {
            TEST_FAIL("Hasil komputasi mengembalikan angka nol mutlak.");
        }
    } catch (const std::exception& e) {
        TEST_FAIL(std::string("Gagal menangani komputasi skala besar: ") + e.what());
    }
}

// --- MAIN RUNNER ---
int main() {
    std::cout << "====================================================\n";
    std::cout << "        STARTING PARALLEL SYSTEM UNIT TESTS        \n";
    std::cout << "====================================================\n\n";

    test_smoke();               
    test_correctness();         
    test_empty_input();         
    test_single_simulation();   
    test_large_simulation_and_faults();    

    std::cout << "====================================================\n";
    if (total_failed == 0) {
        std::cout << "\033[32mAKHIR REVIU: SEMUA UNIT TEST BERHASIL (PASSED)!\033[0m\n";
    } else {
        std::cout << "\033[31mAKHIR REVIU: TERDAPAT " << total_failed << " TEST YANG GAGAL!\033[0m\n";
    }
    std::cout << "====================================================\n";

    return total_failed;
}