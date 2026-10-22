/*
 * A389291  a(n) = Sum_{k=0..floor(n/4)} binomial(n+k-1,k) *
 *          binomial(k,n-4*k).
 * https://oeis.org/A389291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(n+k-1, k) * binomial(k,
 *            n-4*k)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 64, u128 n <= 125,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* handle the offset case */
    if (n == 0)
        return (seq_t)1;

    idx_t k_max = n / 4;   /* largest k with n - 4*k >= 0 */
    seq_t sum = 0;

    for (idx_t k = 0; k <= k_max; ++k)
    {
        idx_t r = n - 4 * k;
        /* binomial(k, r) is zero when r > k, but guard to avoid
           computing it unnecessarily.  For n>=1 and k<=n/4, r>=0.  */
        if (r <= k)
        {
            seq_t t1 = binomial(n + k - 1, k);
            seq_t t2 = binomial(k, r);
            sum += t1 * t2;
        }
    }

    return sum;
}
