/*
 * A318514  a(n) = n OR (greatest proper divisor of n).
 * https://oeis.org/A318514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n OR (greatest proper divisor of n)
 * Range:     u8 n <= 255, u16 n <= 65535, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 100 terms through n = 9926 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* greatest proper divisor of u (u ≥ 1)
   For u = 1, returns 1.
   For composite u, returns u / smallest prime divisor.
   For prime u, returns 1. */
static uint64_t _greatest_proper_divisor_u64(uint64_t u)
{
    if (u <= 1)
        return 1;
    for (uint64_t d = 2; d <= u / d; ++d) {
        if (u % d == 0)
            return u / d;
    }
    return 1;
}

/* Computes a(n) for n ≥ 1.  For n = 1 the result is 1. */
seq_t a(idx_t n)
{
    /* n fits in uint64_t by definition of idx_t */
    uint64_t u = (uint64_t)n;
    seq_t gpd = (seq_t)_greatest_proper_divisor_u64(u);
    return ((seq_t)u) | gpd;
}
