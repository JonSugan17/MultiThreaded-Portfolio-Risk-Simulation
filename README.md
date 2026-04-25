# Akselerasi Simulasi Monte Carlo dengan OpenMP untuk Estimasi Value at Risk (VaR)

## Deskripsi Proyek
Proyek ini bertujuan untuk mengakselerasi perhitungan Value at Risk (VaR) pada portofolio investasi multiaset menggunakan Simulasi Monte Carlo yang diparalelkan dengan OpenMP.

Metode Monte Carlo dikenal memiliki akurasi tinggi dalam estimasi risiko, namun membutuhkan komputasi besar karena melibatkan ribuan hingga jutaan iterasi. Oleh karena itu, komputasi paralel digunakan untuk meningkatkan performa dan efisiensi.

---

## Tujuan
- Mengimplementasikan simulasi Monte Carlo untuk estimasi VaR
- Mengidentifikasi bottleneck pada proses komputasi
- Menerapkan paralelisasi menggunakan OpenMP
- Membandingkan performa antara metode serial dan paralel
- Mengukur speedup dan efisiensi sistem

---

## Teknologi yang Digunakan
- Bahasa Pemrograman: C / C++
- Parallel Computing: OpenMP
- Compiler: GCC (dengan flag -fopenmp)
- Tools Tambahan: Python (opsional untuk visualisasi)

---

## Konsep Utama

### 1. Value at Risk (VaR)
VaR adalah ukuran statistik untuk memperkirakan kerugian maksimum dalam periode tertentu dengan tingkat kepercayaan tertentu.

### 2. Monte Carlo Simulation
Metode berbasis simulasi acak untuk menghasilkan distribusi kemungkinan return portofolio.

### 3. Parallel Computing (OpenMP)
Digunakan untuk membagi beban simulasi ke beberapa thread sehingga mempercepat eksekusi.

---

## Alur Sistem
1. Input data harga historis aset  
2. Menghitung return masing-masing aset  
3. Simulasi Monte Carlo (generate random return)  
4. Hitung return portofolio  
5. Ulangi hingga N iterasi  
6. Ambil quantile sebagai VaR  
7. Bandingkan performa serial vs paralel  

---

## Bottleneck
- Proses simulasi berulang (loop besar)
- Waktu komputasi tinggi pada eksekusi serial
- Skalabilitas terbatas tanpa paralelisme

---

## Solusi
Menggunakan OpenMP untuk:
- Paralelisasi loop simulasi
- Distribusi workload ke multiple thread
- Mengurangi waktu eksekusi secara signifikan

---

## Evaluasi Kinerja

### Speedup
```
Speedup = T_serial / T_parallel
```

### Efficiency
```
Efficiency = Speedup / Jumlah Thread
```

---

## Hasil yang Diharapkan
- Penurunan waktu komputasi secara signifikan
- Speedup mendekati linear
- Efisiensi penggunaan thread tinggi
- Akurasi VaR tetap konsisten

---

## Cara Menjalankan

### Compile
```
gcc -fopenmp main.c -o var_simulation
```

### Run
```
./var_simulation
```

---

## Struktur Folder (Contoh)
```
├── main.c
├── data/
│   └── harga_saham.csv
├── docs/
│   └── laporan.docx
├── results/
│   └── output.txt
└── README.md
```

---

## Referensi
- Monte Carlo Simulation untuk VaR
- Parallel Computing dengan OpenMP
- Literatur terkait Value at Risk

---

## Author
Fredrik Danielsen  
Benaya Abelio
Jonathan Sugan
Muhamad Ridha Safari

---

## Catatan
Proyek ini dapat dikembangkan lebih lanjut dengan:
- Implementasi GPU (CUDA)
- Penggunaan dataset real-time
- Visualisasi hasil (grafik distribusi VaR)
