/*
 * A382306  a(n) is the number of values m that satisfy floor(sqrt(m))=n and
 *          A382286(m)=1.
 * https://oeis.org/A382306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{m : n^2 <= m <= n^2+2*n and C(m)=0}|
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A382306: number of m with floor(sqrt(m)) = n and A382286(m)=1 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for 0 <= x < 2^64 */
static idx_t floor_sqrt(idx_t x)
{
    idx_t low  = 0;
    idx_t high = ((idx_t)1 << 32) - 1;  /* sqrt(2^64-1) < 2^32 */
    idx_t ans  = 0;
    while (low <= high)
    {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq <= x)
        {
            ans = mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break;  /* avoid underflow */
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* n is the index (1‑based). */
    idx_t sq = floor_sqrt(n);          /* floor(sqrt(n)) */
    idx_t start = sq * sq;             /* n^2 */
    idx_t t = n - start;               /* offset within the block, 0 <= t <= 2*sq */

    if (t < sq)
    {
        /* a(n^2 + t) = 3 + 2*t  for 0 <= t <= sq-1 */
        return (seq_t)3 + (seq_t)2 * (seq_t)t;
    }
    else if (t == (sq << 1))
    {
        /* a(n^2 + 2*sq) = 1 */
        return (seq_t)1;
    }
    else
    {
        /* a(n^2 + sq + m) = 2*(sq - m)  where m = t - sq, 0 <= m <= sq-1 */
        idx_t m = t - sq;
        return (seq_t)2 * (seq_t)(sq - m);
    }
}
