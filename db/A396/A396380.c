/*
 * A396380  Number of integers m in {n, n+1, n+2, ..., n+A396377(n)} so that m
 *          divides the product of the other integers.
 * https://oeis.org/A396380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A396380.
   For each n (offset 1) find the smallest k≥0 such that
   in the interval [n, n+k] there exists at least one m with
   m | Π_{i∈[n,n+k], i≠m} i, and then return the number of such m. */

#include <stdbool.h>

seq_t a(idx_t n) {
    for (idx_t k = 0; ; ++k) {
        seq_t count = 0;
        bool found = false;
        for (idx_t m_off = 0; m_off <= k; ++m_off) {
            idx_t m = n + m_off;
            idx_t prod = 1 % m; /* handle m == 1 */
            for (idx_t j_off = 0; j_off <= k; ++j_off) {
                if (j_off == m_off) continue;
                idx_t j = n + j_off;
                prod = (prod * j) % m;
                if (prod == 0) break; /* product already divisible by m */
            }
            if (prod == 0) {
                ++count;
                found = true;
            }
        }
        if (found) return (seq_t)count;
    }
}
