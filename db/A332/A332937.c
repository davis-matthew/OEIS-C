/*
 * A332937  a(n) is the greatest common divisor of the first two terms of row
 *          n of the Wythoff array (A035513).
 * https://oeis.org/A332937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd( floor(n φ), floor( floor(n φ) φ ))
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 6057, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 124 terms through n = 9378 (2026-11-01)
 * Helpers:   gcd, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where φ = (1+√5)/2.  For n≥1 we compute the lower Beatty sequence
   term a_n = ⌊n φ⌋ as (n + ⌊√(5 n²)⌋)/2 and then a_{a_n} similarly.
   The gcd of these two terms is the desired a(n). */
#include <stdint.h>
#include "../utils/gcd.h"
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
    if (n < 1) return 0;                   /* offset is 1 */
    seq_t ns = (seq_t)n;

    /* a_n = floor(n φ) = (n + floor(sqrt(5 n²)))/2 */
    seq_t sq1 = floor_sqrt(ns * ns * 5);
    seq_t an  = (ns + sq1) / 2;

    /* a_{a_n} = floor(a_n φ) similarly */
    seq_t sq2 = floor_sqrt(an * an * 5);
    seq_t aan = (an + sq2) / 2;

    return gcd(an, aan);
}
