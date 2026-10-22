/*
 * A347802  Expansion of ( Sum_{k>=0} k^2 * q^(k^2) )^3.
 * https://oeis.org/A347802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i,j,k>0, i^2+j^2+k^2=n} i^2*j^2*k^2
 * Range:     u8 n <= 16, u16 n <= 76, u32 n <= 1741, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Integer square root for uint64_t using binary search.
   Returns floor(sqrt(n)). */
static idx_t u64_isqrt(idx_t n)
{
    if (n < 2)
        return n;
    idx_t low = 0;
    idx_t high = n;
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            ans = 0;
            low = 1;
            continue;
        }
        /* avoid overflow: mid*mid > n iff mid > n/mid */
        if (mid > n / mid) {
            high = mid - 1;
        } else {
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_i = u64_isqrt(n);
    for (idx_t i = 1; i <= max_i; ++i) {
        idx_t i2 = i * i;               /* i^2 <= n by construction */
        idx_t rem1 = n - i2;
        idx_t max_j = u64_isqrt(rem1);
        for (idx_t j = 1; j <= max_j; ++j) {
            idx_t j2 = j * j;           /* j^2 <= rem1 */
            idx_t rem2 = rem1 - j2;
            idx_t k = u64_isqrt(rem2);
            if (k >= 1 && k * k == rem2) {
                seq_t term = (seq_t)i * i;   /* i^2 */
                term *= (seq_t)j * j;        /* i^2*j^2 */
                term *= (seq_t)k * k;        /* i^2*j^2*k^2 */
                sum += term;
            }
        }
    }
    return sum;
}
