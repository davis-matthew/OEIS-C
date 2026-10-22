/*
 * A375241  Nontriangular numbers with digital root in {1, 3, 6, 9}.
 * https://oeis.org/A375241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th positive integer that is not a triangular number *
 *            and whose digital root is 1, 3, 6, or 9
 * Range:     u8 none, u16 n <= 305, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   desired_dr, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/desired_dr.h"

/* Helper: test whether x is a triangular number.
 * x is triangular iff 1 + 8*x is a perfect square and its square root is odd. */
static inline bool is_triangular(seq_t x)
{
    seq_t d = 1 + (seq_t)8 * x;        /* discriminant */
    seq_t s = floor_sqrt(d);
    return (s * s == d) && (s & 1);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset is 1; 0 is not used but safe */
    idx_t   count = 0;
    seq_t   num   = 0;            /* current integer being tested */

    while (count < n)
    {
        ++num;
        if (desired_dr(num) && !is_triangular(num))
            ++count;
    }
    return num;
}
