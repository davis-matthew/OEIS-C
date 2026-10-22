/*
 * A365716  a(n) = 1 if the largest divisor of n <= sqrt(n) is a power of 2,
 *          otherwise 0.
 * https://oeis.org/A365716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the largest divisor of n ≤ sqrt(n) is a power of 2,
 *            otherwise 0. Offset: 1 Implementation uses integer arithmetic
 *            only
 * Range:     u8 n <= 9629, u16 n <= 9629, u32 n <= 9629, u64 n <= 9629,
 *            u128 n <= 9629, big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_power_of_two.h"   /* to test if a seq_t is a power of 2 */

/* Integer square root for non‑negative 64‑bit values.
   Returns floor(sqrt(n)) without using floating point. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    idx_t mid;

    while (low <= high)
    {
        mid = low + ((high - low) >> 1);
        if (mid == 0)               /* mid^2 == 0 ≤ n */
        {
            low = mid + 1;
            continue;
        }
        if (mid > n / mid)          /* mid^2 > n  (use division to avoid overflow) */
            high = mid - 1;
        else
            low = mid + 1;          /* mid^2 ≤ n */
    }
    return high;                     /* high == floor(sqrt(n)) after loop */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset starts at 1, but handle safely */

    idx_t sq = isqrt_u64(n);         /* floor of sqrt(n) */

    for (idx_t d = sq; d >= 1; --d)
    {
        if (n % d == 0)              /* d is the largest divisor ≤ sqrt(n) */
        {
            return is_power_of_two((seq_t)d) ? (seq_t)1 : (seq_t)0;
        }
    }
    return (seq_t)0;                 /* unreachable for n ≥ 1 */
}
