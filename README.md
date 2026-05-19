Berikut adalah versi README.md yang sudah diperbarui dan disesuaikan dengan perintah kompilasi (menggunakan `g++`) serta struktur folder aktual yang ada pada *screenshot* proyek Anda.

---

# 🚀 Akselerasi Simulasi Monte Carlo dengan OpenMP untuk Estimasi Value at Risk (VaR)

## 📌 Deskripsi Proyek

Proyek ini bertujuan untuk mengakselerasi perhitungan Value at Risk (VaR) pada portofolio investasi multiaset menggunakan Simulasi Monte Carlo yang diparalelkan dengan OpenMP.
Metode Monte Carlo dikenal memiliki akurasi tinggi dalam estimasi risiko, namun membutuhkan komputasi besar karena melibatkan ribuan hingga jutaan iterasi. Oleh karena itu, komputasi paralel digunakan untuk meningkatkan performa dan efisiensi waktu eksekusi.

## 🎯 Tujuan

* Mengimplementasikan simulasi Monte Carlo untuk estimasi VaR.
* Mengidentifikasi *bottleneck* pada proses komputasi.
* Menerapkan paralelisasi menggunakan OpenMP.
* Membandingkan performa antara metode serial dan paralel.
* Mengukur *speedup* dan efisiensi sistem.

## ⚙️ Teknologi yang Digunakan

* **Bahasa Pemrograman:** C++
* **Parallel Computing:** OpenMP
* **Compiler:** GCC/G++ (dengan flag `-fopenmp` dan optimasi `-O3`)
* **Tools Tambahan:** Python (opsional untuk *generate* data/visualisasi)

## 🧠 Konsep Utama

1. **Value at Risk (VaR)**
Ukuran statistik untuk memperkirakan kerugian maksimum dalam periode tertentu dengan tingkat kepercayaan tertentu.
2. **Monte Carlo Simulation**
Metode berbasis simulasi acak untuk menghasilkan distribusi kemungkinan *return* portofolio.
3. **Parallel Computing (OpenMP)**
Digunakan untuk membagi beban simulasi ke beberapa *thread* sehingga mempercepat proses eksekusi iterasi yang masif.

## 🏗️ Alur Sistem

1. Input data parameter (Cholesky matrix, means, weights).
2. *Generate random return* berbasis distribusi normal.
3. Hitung *return* portofolio menggunakan hasil dekomposisi Cholesky.
4. Ulangi simulasi Monte Carlo hingga $N$ iterasi.
5. Urutkan hasil dan ambil *quantile* sebagai nilai VaR.
6. Bandingkan performa eksekusi komputasi serial vs paralel.

## 🔥 Bottleneck & ⚡ Solusi

* **Bottleneck:** Proses simulasi memiliki *loop* iterasi yang sangat besar, menyebabkan waktu komputasi tinggi dan skalabilitas terbatas pada eksekusi serial.
* **Solusi:** Menggunakan OpenMP untuk memparalelkan *loop* simulasi, mendistribusikan *workload* ke *multiple thread*, dan menekan waktu eksekusi secara signifikan.

## 📊 Evaluasi Kinerja

* **Speedup:** $Speedup = T_{serial} / T_{parallel}$
* **Efficiency:** $Efficiency = Speedup / Jumlah Thread$

## 📋 Prerequisites

Sebelum menjalankan proyek ini, pastikan sistem Anda memiliki:

* **OS:** Windows / Linux / macOS
* **Compiler:** G++ dengan dukungan OpenMP
* **Opsional:** Python 3.8+ (untuk skrip `generate_data.py`)

Cek ketersediaan G++ dan OpenMP:

```bash
g++ --version

```

## 🛠️ Cara Kompilasi dan Menjalankan

Proyek ini menggunakan beberapa perintah kompilasi untuk tujuan yang berbeda (program utama, pengujian simulasi, dan *unit test*). Pastikan direktori `bin` sudah terbuat sebelum melakukan kompilasi.

**1. Kompilasi Program Utama (VaR Simulation)**

```bash
g++ -O3 -fopenmp -Iinclude src/parser.cpp src/simulation.cpp src/main.cpp -o bin/var_simulation

```

> **Run:** `./bin/var_simulation` (atau `.\bin\var_simulation.exe` di Windows)

**2. Kompilasi Test Simulation**

```bash
g++ -O3 -fopenmp -Iinclude src/simulation.cpp src/test_simulation.cpp -o bin/test_simulation

```

> **Run:** `./bin/test_simulation` (atau `.\bin\test_simulation.exe` di Windows)

**3. Kompilasi Unit Testing**

```bash
g++ -O3 -fopenmp -Iinclude src/simulation.cpp tests/unit_testing.cpp -o bin/unit_test

```

> **Run:** `./bin/unit_test` (atau `.\bin\unit_test.exe` di Windows)

## 📁 Struktur Direktori

```text
MultiThreaded-Portfolio-Risk-Simulation-3.0-week3/
│
├── bin/                        # Folder output hasil kompilasi (.exe / binary)
│   ├── var_simulation
│   ├── test_simulation
│   └── unit_test
│
├── data/                       # Dataset / Parameter input (CSV)
│   ├── cholesky_matrix.csv
│   ├── means.csv
│   └── weights.csv
│
├── include/                    # Header files (.hpp)
│   ├── parser.hpp
│   └── simulation.hpp
│
├── scripts/                    # Skrip pendukung
│   └── generate_data.py
│
├── src/                        # Source code utama (C++)
│   ├── hello_world_omp.cpp
│   ├── main.cpp
│   ├── parser.cpp
│   ├── simulation.cpp
│   └── test_simulation.cpp
│
├── tests/                      # Skrip unit testing
│   └── unit_testing.cpp
│
├── .gitignore                  # File yang diabaikan oleh Git
└── README.md                   # Dokumentasi proyek

```

## 👨‍💻 Author

* Fredrik Danielsen
* Benaya Abelio
* Jonathan Sugan
* Muhamad Ridha Safari

## 💡 Catatan Pengembangan

Proyek ini dapat dikembangkan lebih lanjut dengan:

* Implementasi akselerasi GPU (CUDA/OpenCL).
* Integrasi *pipeline* data *real-time* untuk pengambilan harga aset langsung dari API finansial.
* Visualisasi distribusi VaR otomatis menggunakan Python/Matplotlib setelah simulasi C++ selesai dijalankan.
