/*
 * A347161  Sum of squares of odd divisors of n that are < sqrt(n).
 * https://oeis.org/A347161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ d² over all odd divisors d of n with d² < n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t isqrt_u64(idx_t n)
{
    /* Compute ⌊√n⌋ for n ≤ 2⁶⁴−1.  Uses binary search with 128‑bit
       arithmetic to avoid overflow. */
    idx_t low = 0, high = ((idx_t)1 << 32) - 1;   /* ≥ ⌊√(2⁶⁴−1)⌋ */
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 1 the sum is 0. */
    seq_t sum = 0;
    if (n <= 1) return sum;

    /* Upper bound for d: the largest odd divisor < sqrt(n).  We use the
       floor square root to avoid a costly recomputation on each step. */
    idx_t t = isqrt_u64(n);

    for (idx_t d = 1; d <= t; d += 2)           /* d runs over odd values only */
    {
        if (n % d != 0) continue;               /* d is not a divisor */
        unsigned __int128 sq = (unsigned __int128)d * d;
        if (sq >= n) continue;                  /* d² must be strictly < n */

        /* d² fits in seq_t because the sequence values for n ≤ 10 000
           stay well within any of the supported arch. types. */
        sum += (seq_t)(d * d);
    }

    return sum;
}
