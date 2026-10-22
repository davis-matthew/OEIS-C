/*
 * A378464  a(n) = Sum_{k=0..floor(n/3)} binomial(n+k-1,k) *
 *          binomial(2*n-1,n-3*k).
 * https://oeis.org/A378464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)}
 *            binomial(n+k-1,k)*binomial(2*n-1,n-3*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                /* a(0) = 1 */

    seq_t total = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t t1 = binomial(n + k - 1, k);
        seq_t t2 = binomial(2 * n - 1, n - 3 * k);
        total += t1 * t2;
    }
    return total;
}
