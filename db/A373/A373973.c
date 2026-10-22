/*
 * A373973  a(n) = 1 if n can be expressed as (a^3 + b^3)/(a*b) with b > a >=
 *          1, otherwise 0.
 * https://oeis.org/A373973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n has a divisor that is a sum of two distinct
 *            positive cubes, 0 otherwise
 * Range:     u8 n <= 1613, u16 n <= 9242, u32 n <= 9242, u64 n <= 9242,
 *            u128 n <= 9242, big any n
 * Helpers:   cube_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/cube_uint64.h"

/* Integer cube root: largest r such that r^3 <= n */
static inline uint64_t int_cuberoot(uint64_t n)
{
    if (n < 8) return (uint64_t)pow((double)n, 1.0/3.0); /* quick return for small numbers */

    double rd = pow((double)n, 1.0/3.0);
    uint64_t r = (uint64_t)rd;
    /* adjust up/down to ensure r^3 <= n < (r+1)^3 */
    while (cube_uint64(r + 1) <= (seq_t)n) ++r;
    while (cube_uint64(r)   >  (seq_t)n) --r;
    return r;
}

/* Test whether a number d is a sum of two distinct positive cubes */
static bool has_cube_sum(uint64_t d)
{
    /* loop over the smaller cube a */
    for (uint64_t a = 1;; ++a)
    {
        seq_t a3 = cube_uint64(a);
        if (a3 >= (seq_t)d) break;

        uint64_t rem = d - a;
        /* rem is d - a^3; but it must fit in uint64_t */
        rem = d - (uint64_t)a3;

        if (rem <= a) break; /* because b must be > a */

        uint64_t b = int_cuberoot(rem);
        if (b == a) continue;              /* distinctness required */
        if (cube_uint64(b) == (seq_t)rem)  /* exact match */
            return true;
    }
    return false;
}

/* Check whether n has at least one divisor that is a sum of two distinct cubes */
static bool has_cube_divisor(uint64_t n)
{
    for (uint64_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            uint64_t d1 = i;
            if (has_cube_sum(d1)) return true;

            uint64_t d2 = n / i;
            if (d2 != d1 && has_cube_sum(d2)) return true;
        }
    }
    return false;
}

/* External linked function required by the build system */
seq_t a(idx_t n)
{
    /* n is an index starting at 1; the OEIS offset is 1 */
    if (n < 1) return 0;
    return (seq_t)(has_cube_divisor((uint64_t)n) ? 1 : 0);
}
