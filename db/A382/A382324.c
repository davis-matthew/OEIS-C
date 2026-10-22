/*
 * A382324  a(n) = least integer h >= 1 such that n is a sum of the form
 *          Sum_{k>=0} floor(h/m^k) for some integer m >= 2.
 * https://oeis.org/A382324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   series_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = least integer h ≥ 1 such that n can be written as  
   Σ_{k≥0} ⌊h/mⁿ⌋ for some integer m ≥ 2.  */
/* The function computes the value by testing h = 1 … n and, for each h, 
   binary‑searching the base m that would give the target sum.  */

#include <stdbool.h>
#include "../utils/series_sum.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n < 1, but make it safe */

    seq_t target = (seq_t)n;

    for (idx_t h = 1; h <= n; ++h)
    {
        seq_t hs = (seq_t)h;
        seq_t maxSum = series_sum(hs, 2);   /* largest possible sum for this h */
        if (maxSum < target)
            continue;                         /* no base m can reach target */

        /* Binary search over m ∈ [2, h+1] (m = h+1 gives sum = h) */
        idx_t low = 2;
        idx_t high = h + 1;                   /* inclusive upper bound */

        while (low <= high)
        {
            idx_t mid = low + (high - low) / 2;
            seq_t s = series_sum(hs, mid);

            if (s == target)
                return hs;                    /* found minimal h with this m */

            if (s > target)
                low = mid + 1;                /* need larger m to reduce sum */
            else
                high = mid - 1;               /* need smaller m to increase sum */
        }
        /* No suitable m for this h, try next h */
    }

    /* Should never reach here for n ≥ 1, but return n as a safe fallback. */
    return target;
}
