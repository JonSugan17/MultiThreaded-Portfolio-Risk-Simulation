import numpy as np
import pandas as pd
import os

def generate_synthetic_data(n_assets=10, output_dir="data"):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    print(f"Generating synthetic data for {n_assets} assets...")

    # 1. Mean Returns (Rata-rata return harian)
    means = np.random.uniform(0.0001, 0.001, n_assets)
    
    # 2. Volatilities (Standar deviasi harian)
    vols = np.random.uniform(0.01, 0.03, n_assets)

    # 3. Weights (Bobot portofolio menggunakan Dirichlet, total pasti 1.0)
    weights = np.random.dirichlet(np.ones(n_assets), size=1).flatten()

    # 4. Correlation Matrix (Harus Positive Semi-Definite)
    A = np.random.rand(n_assets, n_assets)
    cov_matrix = np.dot(A, A.T)
    D = np.sqrt(np.diag(cov_matrix))
    corr_matrix = cov_matrix / np.outer(D, D)

    # 5. Cholesky Decomposition dengan Pengaman Numerik
    try:
        cholesky_matrix = np.linalg.cholesky(corr_matrix)
    except np.linalg.LinAlgError:

        print("[!] Menambahkan jitter numerik kecil untuk menstabilkan matriks korelasi...")
        corr_matrix += np.eye(n_assets) * 1e-8
        cholesky_matrix = np.linalg.cholesky(corr_matrix)

    pd.DataFrame(means, columns=['mean']).to_csv(f"{output_dir}/means.csv", index=False)
    pd.DataFrame(weights, columns=['weight']).to_csv(f"{output_dir}/weights.csv", index=False)
    pd.DataFrame(cholesky_matrix).to_csv(f"{output_dir}/cholesky_matrix.csv", index=False, header=False)

    print(f"Done! Files saved in /{output_dir}\n")

if __name__ == "__main__":
    # Memastikan data yang dihasilkan selalu konsisten untuk komparasi performa sekuensial vs paralel 
    np.random.seed(42) 
    
    generate_synthetic_data(n_assets=1000)
