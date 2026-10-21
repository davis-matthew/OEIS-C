/*
 * A296589  a(n) = Product_{k=0..n} binomial(2*n, k).
 * https://oeis.org/A296589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0}^{n} binom(2*n, k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    if (n == 0)
        return result;

    seq_t two_n = (seq_t)n * 2;   /* 2*n as seq_t */
    seq_t binom  = (seq_t)1;      /* binomial(2n,0) */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t ek = (seq_t)k;
        binom = binom * ((seq_t)two_n - ek + 1) / ek; /* next binomial */
        result *= binom;
    }

    return result;
}
