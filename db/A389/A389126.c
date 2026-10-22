/*
 * A389126  a(n) = Sum_{k=0..floor(n/4)} binomial(n,k) * binomial(2*k,n-4*k).
 * https://oeis.org/A389126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binomial(n,k) * binomial(2*k, n-4*k)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 60, u128 n <= 116,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /*  n/4  is integer division (floor) for unsigned types */
    for (idx_t k = 0; k <= n / 4; ++k)
    {
        idx_t double_k = k << 1;          /* 2*k */
        idx_t sub = n - (k << 2);         /* n-4*k (safe because k≤n/4) */

        seq_t b1 = binomial(n, k);
        seq_t b2 = binomial(double_k, sub);

        sum += b1 * b2;
    }

    return sum;
}
