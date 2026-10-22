/*
 * A332350  Triangle read by rows: T(m,n) = Sum_{-m<i<m, -n<j<n, gcd{i,j}=1}
 *          (m-|i|)*(n-|j|), m >= n >= 1.
 * https://oeis.org/A332350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,n) read by rows, where T(m,n) = Σ_{-m<i<m, -n<j<n,
 *            gcd(i,j)=1} (m-|i|)*(n-|j|), m ≥ n ≥ 1. The index n is
 *            interpreted as row‑wise traversal
 * Range:     u8 n <= 13, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Find the row m such that 1 + 2 + … + m ≥ n. */
    idx_t m = 1;
    while ((m * (m + 1) / 2) < n)
        ++m;

    idx_t prev = (m - 1) * m / 2;
    idx_t k = n - prev;            /* n in the current row (1‑based) */

    seq_t result = 0;

    for (idx_t ai = 0; ai < m; ++ai) {
        idx_t mult_i = (ai == 0 ? 1 : 2);
        for (idx_t aj = 0; aj < k; ++aj) {
            if (ai == 0 && aj == 0) continue;  /* gcd(0,0) undefined */

            idx_t mult_j = (aj == 0 ? 1 : 2);
            if (gcd((seq_t)ai, (seq_t)aj) == 1) {
                seq_t term = (seq_t)(m - ai) * (seq_t)(k - aj);
                result += term * ((seq_t)mult_i * (seq_t)mult_j);
            }
        }
    }

    return result;
}
