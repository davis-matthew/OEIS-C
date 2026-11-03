/*
 * A330176  a(n) = n + floor(nr/t) + floor(ns/t), where r = sqrt(5) - 2, s =
 *          sqrt(5) - 1, t = sqrt(5).
 * https://oeis.org/A330176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(nr/t) + floor(ns/t) where * r = sqrt(5) - 2, s
 *            = sqrt(5) - 1, t = sqrt(5). * This implementation rewrites it as
 *            * a(n) = 3n - ceil(2n/√5) - ceil(n/√5) * and evaluates the
 *            ceilings using integer arithmetic
 * Range:     u8 n <= 8, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Guard against the offset: a(0) is unused, but we return 0 safely. */
    if (n == 0) {
        return 0;
    }

    /* Compute n^2 using 128‑bit arithmetic to prevent overflow. */
    unsigned __int128 nn = (unsigned __int128)n * n;

    /* t2 = floor(n^2 / 5),  t1 = floor(4n^2 / 5).          */
    seq_t t2 = (seq_t)(nn / 5ULL);
    seq_t t1 = (seq_t)((nn * 4ULL) / 5ULL);

    /* floor_sqrt computes ⌊√x⌋ accurately for arbitrary precision types. */
    seq_t s2 = floor_sqrt(t2);   /* floor(n/√5)            */
    seq_t s1 = floor_sqrt(t1);   /* floor(2n/√5)           */

    /* a(n) = 3n – 2 – (floor(√t1) + floor(√t2)) */
    unsigned __int128 base   = (unsigned __int128)3 * n - 2;
    unsigned __int128 result = base - (unsigned __int128)s1 - (unsigned __int128)s2;

    return (seq_t)result;
}
