/*
 * A389378  a(n) = Sum_{k=0..floor(n/3)} binomial(n+k-1,k) *
 *          binomial(n+2*k-1,n-3*k).
 * https://oeis.org/A389378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n + k - 1, k) * binomial(n
 *            + 2k - 1, n - 3k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the first few cases where the general formula involves a
       negative lower index in the binomial coefficient. */
    if (n == 0)               return (seq_t)1;
    if (n == 1 || n == 2)     return (seq_t)0;

    idx_t k_max = n / 3;
    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= k_max; ++k)
    {
        seq_t t1 = binomial(n + k - 1, k);
        seq_t t2 = binomial(n + 2 * k - 1, n - 3 * k);
        sum += t1 * t2;
    }

    return sum;
}
