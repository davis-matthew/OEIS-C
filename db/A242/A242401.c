/*
 * A242401  Numbers that are neither triangular nor square.
 * https://oeis.org/A242401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1491, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers that are neither triangular nor square.
   Formula: a(n) = the n-th positive integer that is not a perfect square
   and not a triangular number.
   The generation is done by a simple linear scan, testing each integer
   sequentially.  The test for squareness uses floor_sqrt, and the test
   for triangularity uses the well‑known criterion that a number x is
   triangular iff (1 + 8x) is a perfect square. */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"            /* provides floor_sqrt for seq_t */

static inline bool is_square(seq_t v)
{
    seq_t r = floor_sqrt(v);
    return r * r == v;
}

static inline bool is_triangular(seq_t v)
{
    /* x is triangular iff 1 + 8*x is a perfect square. */
    seq_t d = (seq_t)1 + ((seq_t)8 * v);
    seq_t r = floor_sqrt(d);
    return r * r == d;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; undefined for n < 1 – return 0 */
        return (seq_t)0;

    idx_t found = 0;
    seq_t candidate = (seq_t)1;   /* start from 1; 2 will be the first term */

    while (1) {
        ++candidate;                /* examine next integer */

        if (is_square(candidate) || is_triangular(candidate))
            continue;               /* skip squares and triangulars */

        ++found;
        if ((idx_t)found == n)
            return candidate;       /* the n‑th admissible number */
    }
}
