/*
 * A398644  Number of partitions of n into 3 parts 1 <= x <= y <= z such that
 *          x^2 + y^2 + z^2 = 3*m^2 for some integer m.
 * https://oeis.org/A398644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions 1 ≤ x ≤ y ≤ z, x+y+z=n, with
 *            x²+y²+z² = 3·m² for some integer m
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 64‑bit unsigned values.
   Returns floor(sqrt(v)). */
static inline uint64_t isqrt_u64(uint64_t v)
{
    uint64_t low = 0, high = (v >> 1) + 1;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid * mid <= v)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* x ranges from 1 to floor(n/3) */
    for (idx_t x = 1; x <= n / 3; ++x) {
        /* y ranges from x to floor((n-x)/2) */
        for (idx_t y = x; y <= (n - x) / 2; ++y) {
            idx_t z = n - x - y;
            /* At this point y ≤ z automatically holds */
            uint64_t sumsq = (uint64_t)x * x + (uint64_t)y * y + (uint64_t)z * z;
            if (sumsq % 3 != 0)
                continue;   /* Not divisible by 3 */
            uint64_t m2 = sumsq / 3;   /* Candidate m² */
            uint64_t r = isqrt_u64(m2);
            if (r * r == m2)
                ++count;            /* Valid partition */
        }
    }
    return count;
}
