/*
 * A387765  a(n) = Sum_{k=0..floor(n/2)} 2^k * binomial(2*k+1,2*n-4*k).
 * https://oeis.org/A387765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} 2^k * binomial(2*k+1, 2*n-4*k)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k)
    {
        /* m = 2*n - 4*k */
        idx_t m = (idx_t)2 * n - (idx_t)4 * k;
        if (m > (idx_t)2 * k + (idx_t)1)   /* binomial is zero otherwise */
            continue;
        seq_t t = pow2(k);                  /* 2^k */
        t *= binomial((idx_t)2 * k + (idx_t)1, m);
        sum += t;
    }
    return sum;
}
