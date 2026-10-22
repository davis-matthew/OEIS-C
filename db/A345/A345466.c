/*
 * A345466  a(n) = Product_{k=1..n} binomial(n, floor(n/k)).
 * https://oeis.org/A345466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} binomial(n, floor(n/k))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"  /* to use binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t t = n / k;            /* floor(n/k) */
        seq_t val = binom(n, t);    /* binomial(n, t) */
        prod *= val;
    }
    return prod;                     /* prod is 1 when n==0 */
}
