/*
 * A389423  a(n) = n*binomial(n, n/2) if n is even otherwise
 *          2^(n-1)*binomial(n-1, (n-1)/2).
 * https://oeis.org/A389423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*binomial(n, n/2) if n even, otherwise
 *            2^(n-1)*binomial(n-1, (n-1)/2)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(0) = 0 and the function works for all n >= 0. */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if ((n & 1U) == 0U) {                 /* even n */
        idx_t k = n >> 1;                 /* n/2 */
        seq_t bin = binom(n, k);          /* binomial(n, n/2) */
        return ((seq_t)n) * bin;          /* n * bin */
    } else {                               /* odd n */
        idx_t k = (n - 1) >> 1;           /* (n-1)/2 */
        seq_t bin = binom(n - 1, k);      /* binomial(n-1, (n-1)/2) */
        seq_t pow = pow2(n - 1);          /* 2^(n-1) */
        return pow * bin;                 /* 2^(n-1) * bin */
    }
}
