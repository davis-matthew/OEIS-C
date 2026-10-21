/*
 * A276158  Triangle read by rows: T(n,k) = 6*k*(n + 1 - k) for 0 < k <= n;
 *          for k = 0, T(n,0) = n + 1.
 * https://oeis.org/A276158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n-th element of the triangular array
 * T(m,k) where T(m,0) = m + 1 and T(m,k) = 6 * k * (m + 1 - k) for 1 ≤ k ≤ m.
 * The index n corresponds to row m and column k found by solving
 *   m(m+1)/2 ≤ n < (m+1)(m+2)/2 .
 * Let m = ⌊(√(8n+1) - 1)/2⌋ and k = n - m(m+1)/2.
 * Then a(n) = T(m,k).
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* provides floor_sqrt(seq_t) */

seq_t a(idx_t n) {
    /* Compute m = floor((sqrt(8n+1) - 1)/2) */
    seq_t X = (seq_t)8 * n + 1;            /* 8*n + 1 */
    seq_t sqrtX = floor_sqrt(X);          /* floor(sqrt(X)) */
    seq_t m = (sqrtX - 1) / 2;            /* row index */

    /* prefix count: total elements before row m */
    seq_t prefix = m * (m + 1) / 2;

    /* column index k within row m */
    seq_t k = n - prefix;

    /* Compute T(m,k) */
    if (k == 0) {
        return m + 1;
    } else {
        return 6 * k * (m + 1 - k);
    }
}
