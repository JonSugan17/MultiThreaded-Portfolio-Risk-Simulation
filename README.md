🚀 Akselerasi Simulasi Monte Carlo dengan OpenMP untuk Estimasi Value at Risk (VaR)
📌 Deskripsi Proyek
Proyek ini bertujuan untuk mengakselerasi perhitungan Value at Risk (VaR) pada portofolio investasi multiaset menggunakan Simulasi Monte Carlo yang diparalelkan dengan OpenMP.
Metode Monte Carlo dikenal memiliki akurasi tinggi dalam estimasi risiko, namun membutuhkan komputasi besar karena melibatkan ribuan hingga jutaan iterasi. Oleh karena itu, komputasi paralel digunakan untuk meningkatkan performa dan efisiensi.

🎯 Tujuan

Mengimplementasikan simulasi Monte Carlo untuk estimasi VaR
Mengidentifikasi bottleneck pada proses komputasi
Menerapkan paralelisasi menggunakan OpenMP
Membandingkan performa antara metode serial dan paralel
Mengukur speedup dan efisiensi sistem


⚙️ Teknologi yang Digunakan

Bahasa Pemrograman: C / C++
Parallel Computing: OpenMP
Compiler: GCC (dengan flag -fopenmp)
Tools Tambahan: Python (opsional untuk visualisasi)


🧠 Konsep Utama
1. Value at Risk (VaR)
VaR adalah ukuran statistik untuk memperkirakan kerugian maksimum dalam periode tertentu dengan tingkat kepercayaan tertentu.
2. Monte Carlo Simulation
Metode berbasis simulasi acak untuk menghasilkan distribusi kemungkinan return portofolio.
3. Parallel Computing (OpenMP)
Digunakan untuk membagi beban simulasi ke beberapa thread sehingga mempercepat eksekusi.

🏗️ Alur Sistem

Input data harga historis aset
Menghitung return masing-masing aset
Simulasi Monte Carlo (generate random return)
Hitung return portofolio
Ulangi hingga N iterasi
Ambil quantile sebagai VaR
Bandingkan performa serial vs paralel


🔥 Bottleneck

Proses simulasi berulang (loop besar)
Waktu komputasi tinggi pada eksekusi serial
Skalabilitas terbatas tanpa paralelisme


⚡ Solusi
Menggunakan OpenMP untuk:

Paralelisasi loop simulasi
Distribusi workload ke multiple thread
Mengurangi waktu eksekusi secara signifikan


📊 Evaluasi Kinerja
Speedup
Speedup = T_serial / T_parallel
Efficiency
Efficiency = Speedup / Jumlah Thread

📈 Hasil yang Diharapkan

Penurunan waktu komputasi secara signifikan
Speedup mendekati linear
Efisiensi penggunaan thread tinggi
Akurasi VaR tetap konsisten


📋 Prerequisites
Sebelum menjalankan proyek ini, pastikan sistem Anda memiliki:

OS: Linux / macOS / Windows (dengan WSL)
Compiler: GCC versi 9.0 ke atas (dengan dukungan OpenMP)
Runtime: OpenMP (biasanya sudah termasuk dalam GCC)
Library: libgomp (GNU OpenMP runtime)
Opsional: Python 3.8+ (untuk visualisasi hasil)

Cek ketersediaan GCC dan OpenMP:
bashgcc --version
echo "#include <omp.h>" | gcc -fopenmp -x c - -o /dev/null && echo "OpenMP supported"

🛠️ Cara Instalasi Environment
1. Clone Repository
bashgit clone https://github.com/username/monte-carlo-var.git
cd monte-carlo-var
2. Install GCC & OpenMP (jika belum ada)
Ubuntu / Debian:
bashsudo apt update
sudo apt install gcc build-essential
macOS (via Homebrew):
bashbrew install gcc
Windows (via WSL):
bashsudo apt install gcc build-essential
3. (Opsional) Install Python untuk Visualisasi
bashpip install matplotlib numpy pandas

▶️ Cara Menjalankan
Compile
bashgcc -fopenmp -O2 src/main.c -o var_simulation -lm
Run — Mode Sequential (Serial)
bash./var_simulation --mode sequential
Run — Mode Parallel (OpenMP)
bash./var_simulation --mode parallel --threads 4
Jumlah thread dapat disesuaikan dengan jumlah core CPU Anda:
bash# Menggunakan semua core yang tersedia
./var_simulation --mode parallel --threads $(nproc)

🧪 Cara Menjalankan Unit Tests
Menggunakan make
bashmake test
Menggunakan pytest (Python — opsional)
bashpytest scripts/test_var.py -v

📁 Struktur Direktori
monte-carlo-var/
├── data/                   # Dataset harga historis aset (CSV)
│   └── harga_saham.csv
├── include/                # Header files (.h)
│   ├── monte_carlo.h
│   └── var_calculator.h
├── scripts/                # Skrip pendukung (Python untuk visualisasi & testing)
│   ├── visualize_var.py
│   └── test_var.py
├── src/                    # Source code utama (C/C++)
│   ├── main.c
│   ├── monte_carlo.c
│   └── var_calculator.c
├── .gitignore              # File yang diabaikan Git
└── README.md               # Dokumentasi proyek
Folder / FileDeskripsidata/Menyimpan data harga historis aset dalam format CSVinclude/Header files untuk deklarasi fungsi dan struktur datascripts/Skrip Python untuk visualisasi distribusi VaR dan unit testingsrc/Implementasi utama simulasi Monte Carlo dan kalkulasi VaR.gitignoreMengecualikan file binary dan output dari version controlREADME.mdDokumentasi lengkap proyek

📚 Referensi

Monte Carlo Simulation untuk VaR
Parallel Computing dengan OpenMP
Literatur terkait Value at Risk


👨‍💻 Author
Fredrik Danielsen
Benaya Abelio
Jonathan Sugan
Muhamad Ridha Safari

💡 Catatan
Proyek ini dapat dikembangkan lebih lanjut dengan:

Implementasi GPU (CUDA)
Penggunaan dataset real-time
Visualisasi hasil (grafik distribusi VaR)
