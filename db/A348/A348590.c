/*
 * A348590  Number of endofunctions on [n] with exactly one isolated fixed
 *          point.
 * https://oeis.org/A348590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Σ_{k=0}^{n-1} (-1)^k * C(n-1,k) * (n-1-k)^(n-1-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* The types seq_t and idx_t are provided by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* a(0) = 0 by definition */

    idx_t m = n - 1;                   /* m = n-1 */
    seq_t sum   = (seq_t)0;
    seq_t binom = (seq_t)1;            /* binom = C(m,0) */

    for (idx_t k = 0; k <= m; ++k) {
        idx_t j = m - k;               /* j = m - k */

        seq_t pow_val;
        if (j == 0)                   /* 0^0 is defined as 1 for the sum */
            pow_val = (seq_t)1;
        else
            pow_val = big_pow((seq_t)j, (seq_t)j);

        seq_t term = binom * pow_val;

        /* add or subtract the term according to the parity of k */
        if ((k & 1) == 0)
            sum += term;
        else
            sum -= term;

        /* update binom for the next k: C(m,k+1) = C(m,k) * (m-k) / (k+1) */
        if (k < m) {
            binom = binom * (seq_t)(m - k);
            binom = binom / (seq_t)(k + 1);
        }
    }

    return (seq_t)n * sum;
}
