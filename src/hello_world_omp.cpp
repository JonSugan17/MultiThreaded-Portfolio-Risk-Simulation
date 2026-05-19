#include <iostream>
#include <omp.h>
#include <unistd.h> // Untuk fungsi sleep/jeda

int main() {
    const int num_tasks = 4; // Jumlah tugas yang akan dikerjakan

    std::cout << "Mulai Eksekusi Program...\n\n";

    // 1. SEQUENTIAL BASELINE (Berurutan)
    std::cout << "=== [ SEQUENTIAL BASELINE ] ===\n";
    double start_seq = omp_get_wtime(); // Mulai catat waktu

    for (int i = 0; i < num_tasks; ++i) {
        // Simulasi beban kerja dengan jeda 1 detik
        sleep(1); 
        std::cout << "Hello World (Sequential) -> Tugas " << i << " selesai.\n";
    }

    double end_seq = omp_get_wtime(); // Selesai catat waktu
    double time_seq = end_seq - start_seq;
    std::cout << "Waktu eksekusi Sequential: " << time_seq << " detik\n\n";


    // 2. PARALLEL EXECUTION (OpenMP)
    std::cout << "=== [ PARALLEL OPENMP ] ===\n";
    double start_par = omp_get_wtime(); // Mulai catat waktu

    // Membagi loop for ke beberapa thread yang tersedia
    #pragma omp parallel for
    for (int i = 0; i < num_tasks; ++i) {
        // Simulasi beban kerja dengan jeda 1 detik
        sleep(1); 
        
        int thread_id = omp_get_thread_num(); // Mendapatkan ID Thread yang sedang bekerja
        
        // #pragma omp critical digunakan agar output teks di terminal 
        // tidak bertabrakan/berantakan antar thread
        #pragma omp critical
        {
            std::cout << "Hello World (Parallel) -> Tugas " << i 
                      << " diselesaikan oleh Thread ID: " << thread_id << "\n";
        }
    }

    double end_par = omp_get_wtime(); // Selesai catat waktu
    double time_par = end_par - start_par;
    std::cout << "Waktu eksekusi Parallel  : " << time_par << " detik\n";

    // 3. KESIMPULAN
    std::cout << "\n=== [ KESIMPULAN ] ===\n";
    std::cout << "Speedup (Peningkatan Kecepatan): " << time_seq / time_par << "x lebih cepat!\n";

    return 0;
}
