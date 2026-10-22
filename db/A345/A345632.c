/*
 * A345632  Sum of terms of even index in the binomial decomposition of
 *          n^(n-1).
 * https://oeis.org/A345632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊(n-1)/2⌋} (n-1)^(2k) * binomial(n-1, 2k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A345632. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based; compute with m = n-1. */
    idx_t m_idx = n - 1;          /* idx_t value */
    seq_t m = (seq_t)m_idx;       /* convert to seq_t for arithmetic */

    /* Base case: n = 1 -> a(1) = 1 */
    seq_t sum = 1;
    if (m_idx == 0)
        return sum;

    /* Compute n^2 once for repeated use */
    seq_t pow2 = m * m;           /* (n-1)^2 */

    seq_t pow = 1;                /* will hold (n-1)^(2k) */
    seq_t binom = 1;              /* binomial(n-1, 0) = 1 */

    for (idx_t j = 1; j <= m_idx; ++j) {
        /* binomial(n-1, j) = binomial(n-1, j-1) * (m-j+1) / j */
        binom = binom * (seq_t)(m_idx - j + 1) / (seq_t)j;

        if ((j & 1) == 0) {      /* even j => contributes a term */
            pow = pow * pow2;    /* update (n-1)^(2k) */
            sum = sum + pow * binom;
        }
    }

    return sum;
}
