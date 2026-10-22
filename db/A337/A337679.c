/*
 * A337679  Number of distinct positive integer pairs, (s,t), such that s < t
 *          < n where neither s nor t divides n, and (s + t) | n.
 * https://oeis.org/A337679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of pairs (s,t) with 1 < s < t < n,
   s and t not dividing n, and (s + t) | n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* No pairs possible for n ≤ 3 */
    if (n <= 3)
        return (seq_t)0;

    seq_t cnt = 0;

    /* Enumerate all divisors d of n */
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0)
            continue;

        /* Process divisor d */
        if (d >= 3)
        {
            idx_t half = (d - 1) / 2;
            for (idx_t s = 1; s <= half; ++s)
            {
                idx_t t = d - s;          /* always s < t */
                if (n % s != 0 && n % t != 0)
                    cnt += (seq_t)1;
            }
        }

        /* Process paired divisor d2 = n / d, avoiding duplication */
        idx_t d2 = n / d;
        if (d2 != d && d2 >= 3)
        {
            idx_t half = (d2 - 1) / 2;
            for (idx_t s = 1; s <= half; ++s)
            {
                idx_t t = d2 - s;
                if (n % s != 0 && n % t != 0)
                    cnt += (seq_t)1;
            }
        }
    }

    return cnt;
}
