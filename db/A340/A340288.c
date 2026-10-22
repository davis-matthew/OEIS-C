/*
 * A340288  a(n) = Sum_{k=1..n} floor(floor((n-k)^(1/k))/k).
 * https://oeis.org/A340288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(floor((n-k)^(1/k))/k)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer k-th root of n: largest m with m^k <= n. */
static seq_t kth_root(idx_t n, idx_t k)
{
    if (n < 2)
        return n;               /* 0 or 1 */
    idx_t lo = 0, hi = n;       /* hi <= n is safe (n <= 10000 for this sequence) */
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;   /* mid is (lo+hi)/2 rounding up */
        seq_t power = 1;
        bool overflow = false;
        /* compute mid^k with early exit on overflow or exceeding n */
        for (idx_t i = 0; i < k; ++i)
        {
            if (mid == 0) {          /* 0^k == 0 */
                power = 0;
                break;
            }
            if (power > n / mid) {   /* would overflow or exceed n */
                overflow = true;
                break;
            }
            power *= mid;
        }
        if (!overflow && power <= n)
            lo = mid;               /* mid^k <= n, try larger */
        else
            hi = mid - 1;           /* mid^k > n, try smaller */
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t rem = n - k;                 /* n-k can be 0 */
        seq_t m = kth_root(rem, k);        /* floor((n-k)^(1/k)) */
        sum += m / (seq_t)k;               /* floor(m/k) */
    }
    return sum;
}
