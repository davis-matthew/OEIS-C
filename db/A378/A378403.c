/*
 * A378403  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+2*k,n-k).
 * https://oeis.org/A378403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+2*k,n-k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t t1 = binomial(n, k);
        idx_t n_plus_2k = n + k + k;     /* n + 2*k, safe for moderate n */
        seq_t t2 = binomial(n_plus_2k, n - k);
        sum += t1 * t2;
    }
    return sum;
}
