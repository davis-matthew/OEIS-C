/*
 * A349906  Number of factorizations of n into even factors > 1 (a(1) = 1 by
 *          convention).
 * https://oeis.org/A349906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9198, u16 n <= 9198, u32 n <= 9198, u64 n <= 9198,
 *            u128 n <= 9198, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Definition: a(n) = number of unordered factorizations of n into
    factors > 1, all of which are even.  (a(1) = 1 by convention.)  */

#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"

static seq_t rec(idx_t n, idx_t min_factor)
{
    if (n < min_factor)          /* no factor ≥ min_factor possible */
        return 0;

    seq_t count = 0;

    /* count the factorization consisting of the single factor n itself,
       if n is even and ≥ min_factor. */
    if ((n & 1U) == 0U && n >= min_factor)
        count += 1;

    /* iterate over even divisors d of n that are ≥ min_factor
       and not larger than sqrt(n); this guarantees non‑decreasing
       order of factors and avoids duplicate factorizations. */
    const idx_t limit = _floor_isqrt_u64(n);
    for (idx_t d = min_factor; d <= limit; d += 2U) {
        if ((n % d) == 0U) {
            count += rec(n / d, d);
        }
    }
    return count;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;          /* convention */
    return rec(n, 2U);
}
