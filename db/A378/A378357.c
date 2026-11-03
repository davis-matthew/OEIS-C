/*
 * A378357  Distance from n to the least non perfect power >= n.
 * https://oeis.org/A378357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = distance from n to the least non perfect power >= n
 *            (returns 0,1,2 because only the pair 8,9 are adjacent perfect
 *            powers)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   ipow_u64_to_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_u64_to_u64.h"

/* Compute integer exponent with 128‑bit accumulation.
   Returns true if base^exp <= limit, else false (overflow or > limit). */
static bool pow_le_uint64(uint64_t base, uint64_t exp, uint64_t limit)
{
    if (exp == 0) return 1 <= limit;
    if (base == 0) return 0 <= limit;
    unsigned __int128 prod = 1;
    while (exp--)
    {
        prod *= base;
        if (prod > limit) return false;
    }
    return true;
}

/* Integer root: greatest r such that r^exp <= n. */
static uint64_t integer_root_uint64(uint64_t n, uint64_t exp)
{
    uint64_t low = 1, high = n;
    while (low < high)
    {
        uint64_t mid = (low + high + 1) >> 1;
        if (pow_le_uint64(mid, exp, n))
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Test whether x is a perfect power (>=1). 1 is considered a perfect power. */
static bool is_perfect_power_uint64(uint64_t x)
{
    if (x <= 1) return true; /* 0,1 are perfect powers by definition */
    for (uint64_t exp = 2; exp <= 64; ++exp)
    {
        uint64_t base = integer_root_uint64(x, exp);
        /* Check base and base+1 (due to rounding). */
        if (ipow_u64_to_u64(base, exp) == x) return true;
        if (base > 0 && ipow_u64_to_u64(base + 1, exp) == x) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1; value for n<1 undefined */
    uint64_t x = (uint64_t)n;

    /* If n itself is not a perfect power, distance is 0. */
    if (!is_perfect_power_uint64(x))
        return (seq_t)0;

    /* Check next integers up to x+2.  All distances are <=2. */
    if (x <= UINT64_MAX - 1 && !is_perfect_power_uint64(x + 1))
        return (seq_t)1;
    if (x <= UINT64_MAX - 2 && !is_perfect_power_uint64(x + 2))
        return (seq_t)2;

    /* For completeness, though this should never be reached. */
    return (seq_t)0;
}
