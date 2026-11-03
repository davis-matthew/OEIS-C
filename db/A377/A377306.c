/*
 * A377306  Numbers that are not the sum of the first k fourth powers.
 * https://oeis.org/A377306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + m if 30n > m(m-1)(6m^3 + 21m^2 + 31m + 31) a(n) = n +
 *            m - 1 otherwise where m = floor((5n)^(1/5))
 * Range:     u8 n <= 252, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9370 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute y^5 using unsigned __int128 to avoid overflow. */
static unsigned __int128 pow5_u128(uint64_t y)
{
    unsigned __int128 v = y;
    return v * v * v * v * v;
}

/* Integer floor fifth root of an unsigned __int128 value x. */
static uint64_t floor_fifth_root_uint128(unsigned __int128 x)
{
    uint64_t low = 0;
    uint64_t high = 1;

    /* Expand the search interval until high^5 > x */
    while (pow5_u128(high) <= x)
    {
        low = high;
        /* Prevent overflow of high while shifting left */
        if (high > ((uint64_t)-1) >> 1)
        {
            high = (uint64_t)-1;
            break;
        }
        high <<= 1;
    }

    /* Binary search in [low, high) */
    while (low + 1 < high)
    {
        uint64_t mid = low + ((high - low) >> 1);
        if (pow5_u128(mid) <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Treat the index as 64‑bit for intermediate calculations */
    unsigned __int128 n128 = (unsigned __int128)n;

    /* Compute m = floor((5n)^(1/5)) */
    unsigned __int128 five_n = 5 * n128;
    uint64_t m = floor_fifth_root_uint128(five_n);
    unsigned __int128 m128 = (unsigned __int128)m;

    /* Compute left side: 30 n */
    unsigned __int128 left = (unsigned __int128)30 * n128;

    /* Compute 6m³ + 21m² + 31m + 31 */
    unsigned __int128 m2 = m128 * m128;
    unsigned __int128 m3 = m2 * m128;
    unsigned __int128 inside = 6 * m3 + 21 * m2 + 31 * m128 + (unsigned __int128)31;

    /* Compute right side: m(m‑1) * inside */
    unsigned __int128 right = m128 * (m128 - (unsigned __int128)1) * inside;

    unsigned __int128 result;
    if (left > right)
        result = n128 + m128;           /* a(n) = n + m */
    else
        result = n128 + m128 - (unsigned __int128)1; /* a(n) = n + m – 1 */

    return (seq_t)result;
}
