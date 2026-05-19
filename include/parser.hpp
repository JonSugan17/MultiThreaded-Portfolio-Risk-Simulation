#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>

class Parser {
public:
    /**
     * Membaca file CSV satu kolom (untuk means.csv dan weights.csv)
     * @param filename Jalur file CSV
     * @param skipHeader Lewati baris pertama jika berisi judul kolom
     * @return Vektor berisi data double
     */
    static std::vector<double> readVector(const std::string& filename, bool skipHeader = true);

    /**
     * Membaca file CSV dua dimensi (untuk cholesky_matrix.csv)
     * @param filename Jalur file CSV
     * @return Matriks (vektor dari vektor) berisi data double
     */
    static std::vector<std::vector<double>> readMatrix(const std::string& filename);

    /**
     * Memvalidasi apakah dimensi data konsisten
     */
    static bool validateDimensions(const std::vector<double>& means, 
                                  const std::vector<double>& weights, 
                                  const std::vector<std::vector<double>>& cholesky);
};

#endif