/*
 * A381337  a(n) is the smallest c >= A381336(n) + n for which a nondegenerate
 *          integer-sided triangle (A381336(n), A381336(n) + n, c) with an
 *          integer area exists.
 * https://oeis.org/A381337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest c >= A381336(n) + n for which the triangle
 *            (A381336(n), A381336(n) + n, c) has integer area. A381336(n) is
 *            the smallest integer a such that there exists c >= a + n with
 *            integer area
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 840, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Necessary UI: seq_t and idx_t types are defined by the build system. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Helper: test whether x is a perfect square.  x must be non‑negative. */
static bool is_square(seq_t x)
{
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

seq_t a(idx_t n)
{
    /* n is >= 1.  Search for the smallest a (denoted `a0`) such that
       a triangle (a0, a0 + n, c) has integer area.  For that a0 find
       the smallest c that works. */
    for (idx_t a0 = 1; ; ++a0)
    {
        seq_t a = (seq_t)a0;          /* side a0 */
        seq_t M = 2 * a + (seq_t)n;   /* M = a + (a + n) = 2a + n */

        /* The third side c must be in [a + n, M - 1] and have the same
           parity as n.  Start from a + n and adjust parity if needed. */
        seq_t start_c = a + (seq_t)n;
        if ( (start_c & 1) != ((seq_t)n & 1) )
            ++start_c;

        for (seq_t c = start_c; c <= M - 1; c += 2)
        {
            /* Compute the product P = (M + c)(n + c)(c - n)(M - c).
               Use __int128 to avoid overflow on 64‑bit builds. */
            __int128 term1 = (__int128)(M + c);
            __int128 term2 = (__int128)(n + c);
            __int128 term3 = (__int128)(c - n);
            __int128 term4 = (__int128)(M - c);
            __int128 prod = term1 * term2 * term3 * term4; /* 128‑bit value */

            /* 16 divides prod because of parity conditions.  If not, skip. */
            if ( (prod & 15) != 0 )
                continue;

            seq_t area2 = (seq_t)(prod / 16); /* area^2 as seq_t */

            if (is_square(area2))
            {
                /* Found the minimal a (a0) and minimal c for that a.  Return. */
                return c;
            }
        }
    }
}
