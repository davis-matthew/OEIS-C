/*
 * A393244  a(n) = Sum_{k=0..floor(n/3)} binomial(2*k,k) * binomial(n,3*k).
 * https://oeis.org/A393244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*k, k) * binomial(n, 3*k)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 25, u64 n <= 49, u128 n <= 96,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"   /* Provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    idx_t maxk = n / 3;                 /* largest k with 3*k <= n          */
    seq_t sum = 0;                      /* accumulate the terms             */

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binomial(2 * k, k);          /* binomial(2k,k)          */
        seq_t t2 = binomial(n, 3 * k);          /* binomial(n,3k)          */
        sum += t1 * t2;                          /* add the product        */
    }

    return sum;
}
