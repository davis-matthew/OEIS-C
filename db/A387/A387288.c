/*
 * A387288  a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) * binomial(2*k,n-2*k).
 * https://oeis.org/A387288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) * binomial(2*k,
 *            n-2*k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t c1 = binom(n, k);                 /* binomial(n,k) */
        seq_t c2 = binom(2 * k, n - 2 * k);     /* binomial(2k,n-2k) */
        sum += c1 * c2;                         /* accumulate the term */
    }

    return sum;
}
