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
 * Helpers:   integer_root_uint64, pow_exact_uint64, pow_le_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/integer_root_uint64.h"
#include "../utils/pow_exact_uint64.h"
#include "../utils/pow_le_uint64.h"

/* Test whether x is a perfect power (>=1). 1 is considered a perfect power. */
static bool is_perfect_power_uint64(uint64_t x)
{
    if (x <= 1) return true; /* 0,1 are perfect powers by definition */
    for (uint64_t exp = 2; exp <= 64; ++exp)
    {
        uint64_t base = integer_root_uint64(x, exp);
        /* Check base and base+1 (due to rounding). */
        if (pow_exact_uint64(base, exp) == x) return true;
        if (base > 0 && pow_exact_uint64(base + 1, exp) == x) return true;
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
