/*
 * A384216  Square array A(n,k), n >= 0, k >= 1, read by antidiagonals: A(n,k)
 *          = n! * [x^n] (1 + k*x)^(n/k).
 * https://oeis.org/A384216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) reads the square array A(n,k) = prod_{j=0}^{n-1} (n - k*j)
 *            (with A(0,k)=1) by antidiagonals
 * Range:     i8 n <= 26, i16 n <= 43, i32 n <= 103, i64 n <= 224,
 *            i128 n <= 488, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Find antidiagonal index s and cumulative count cum
    idx_t s = 0;
    idx_t cum = 0;
    while (cum + s + 1 <= n) {
        ++s;
        cum += s;
    }
    idx_t row = n - cum;          // row index (n in array)
    idx_t col = s - row + 1;      // column index k

    seq_t result = 1;
    // Compute product_{j=0}^{row-1} (row - col*j)
    for (idx_t j = 0; j < row; ++j) {
        result *= (seq_t)(row - (seq_t)col * j);
    }
    return result;
}
