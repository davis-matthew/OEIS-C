/*
 * A392282  Array read by antidiagonals: Place k points in general position on
 *          each side of a regular n-gon and join every pair of the k*n
 *          boundary points by a chord; T(n,k) (n >= 3, k >= 0) gives the
 *          number of edges in the resulting planar graph.
 * https://oeis.org/A392282
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates T(n,k) using the antidiagonal reading of the
 *            array with T(n,k) = k^2*n*(n-1)*(k^2*(n^2+n-3)-6*k*(n-1)+9)/12 +
 *            n*(k+1). For n≥3: idx = n-3 find d such that d*(d+1)/2 ≤ idx <
 *            (d+1)*(d+2)/2 j = idx - d*(d+1)/2 n' = j+3, k' = d-j Return
 *            T(n',k')
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 3) return 0;

    idx_t idx = n - 3;

    /* find d such that d*(d+1)/2 ≤ idx < (d+1)*(d+2)/2 */
    idx_t low = 0, high = 65536;     /* sufficient upper bound for n≈10⁴ */
    idx_t d = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t val = mid * (mid + 1) / 2;
        if (val <= idx) {
            d = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    idx_t prev = d * (d + 1) / 2;
    idx_t j = idx - prev;           /* 0 ≤ j ≤ d */
    idx_t n_val = j + 3;            /* polygon side count */
    idx_t k_val = d - j;            /* points per side */

    /* compute T(n_val, k_val) in unsigned __int128 to avoid overflow */
    unsigned __int128 N = n_val;
    unsigned __int128 K = k_val;
    unsigned __int128 K2 = K * K;
    unsigned __int128 partA = K2 * N * (N - 1);
    unsigned __int128 inside = K2 * (N * N + N - 3) - (6 * K * (N - 1)) + 9;
    unsigned __int128 partB = (partA * inside) / 12;
    unsigned __int128 partC = N * (K + 1);
    unsigned __int128 result = partB + partC;

    return (seq_t)result;
}
