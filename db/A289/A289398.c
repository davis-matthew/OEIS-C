/*
 * A289398  Least integer m > n such that (n^2 + m^2)/2 is a square.
 * https://oeis.org/A289398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 5785, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the least integer m > n such that (n^2 + m^2)/2 is a perfect square. */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Check if a seq_t number is a perfect square. */
static inline bool is_square(seq_t x)
{
    seq_t s = floor_sqrt(x);
    return s * s == x;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset, but safe */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    /* Upper bound: proven that a(n) <= 7*n for all n. */
    idx_t start = n + 1;
    idx_t limit = 7 * n;
    for (idx_t m = start; m <= limit; ++m) {
        seq_t m_sq = (seq_t)m * (seq_t)m;
        seq_t sum  = n_sq + m_sq;
        if ((sum & 1) != 0) continue;    /* sum must be even */
        seq_t t = sum >> 1;
        if (is_square(t))
            return (seq_t)m;
    }
    /* If not found within the proven bound, continue searching unbounded. */
    for (idx_t m = limit + 1;; ++m) {
        seq_t m_sq = (seq_t)m * (seq_t)m;
        seq_t sum  = n_sq + m_sq;
        if ((sum & 1) != 0) continue;
        seq_t t = sum >> 1;
        if (is_square(t))
            return (seq_t)m;
    }
}
