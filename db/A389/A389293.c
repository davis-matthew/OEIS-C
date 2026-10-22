/*
 * A389293  a(n) = Sum_{k=0..floor(n/2)} binomial(n+k-1,k) *
 *          binomial(3*k,n-2*k).
 * https://oeis.org/A389293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n+k-1,k) *
 *            binomial(3*k,n-2*k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* handle the offset n = 0 explicitly to avoid negative arguments
       in binomial(n+k-1,k) */
    if (n == 0)
        return 1;

    seq_t total = 0;
    idx_t maxk = n >> 1;           /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t t1 = binomial(n + k - 1, k);
        seq_t t2 = binomial(3 * k, n - 2 * k);
        total += t1 * t2;
    }
    return total;
}
