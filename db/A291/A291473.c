/*
 * A291473  The arithmetic function u(n,6,8).
 * https://oeis.org/A291473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest divisor of n that is > 5, capped at 41. For
 *            n = 1..5 the values are 41, 34, 27, 34, 41 respectively
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* Special values for n = 1..5 */
    static const seq_t first[5] = { 41, 34, 27, 34, 41 };
    if (n <= 5)
        return first[n - 1];

    /* Start with n itself; for primes this will stay n. */
    seq_t best = (seq_t)n;
    idx_t max = _floor_isqrt_u64(n);

    for (idx_t d = 2; d <= max; ++d) {
        if (n % d == 0) {
            /* Check divisor d */
            if (d > 5 && (seq_t)d < best)
                best = (seq_t)d;

            /* Check complementary divisor e = n/d */
            idx_t e = n / d;
            if (e > 5 && (seq_t)e < best)
                best = (seq_t)e;
        }
    }

    /* Cap at 41 for primes greater than 41. */
    if (best > (seq_t)41)
        best = (seq_t)41;

    return best;
}
