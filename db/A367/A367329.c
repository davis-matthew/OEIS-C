/*
 * A367329  The y-coordinate of the point where x + y = n, x & y are integers
 *          and x/y is as close as possible to e.
 * https://oeis.org/A367329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A367329: y-coordinate of the point where x+y=n with x,y≥0 and
   x/y closest to e.  This implementation evaluates all y from 1
   to n and selects the one that minimizes |(n−y)/y−e|. */

/* Uses integer arithmetic only.  The approximation of e+1 used
   is M/D = 3718281828459045 / 1000000000000000.  All intermediate
   calculations fit in unsigned __int128 for n ≤ 10 000. */

#include <stdint.h>   /* for uint64_t constants */

/* index type (uint64_t) and return type (seq_t) are provided by the build. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* offset = 1, never called with 0 */
        return 0;

    /* constants for (e + 1) ≈ 3.718281828459045 */
    const uint64_t D = 1000000000000000ULL;      /* denominator 10^15      */
    const uint64_t M = 3718281828459045ULL;      /* numerator of e+1       */

    /* n * D  (fits in 128 bits for n up to 10 000) */
    const unsigned __int128 base = (unsigned __int128)n * D;

    /* best candidate so far */
    seq_t best_y = 1;
    unsigned __int128 best_diff =
        (base > (unsigned __int128)M * 1ULL
         ? base - (unsigned __int128)M * 1ULL
         : (unsigned __int128)M * 1ULL - base);

    /* test all possible y values 2 .. n */
    for (idx_t y = 2; y <= n; ++y)
    {
        const unsigned __int128 prod = (unsigned __int128)M * y;
        const unsigned __int128 diff = (base > prod ? base - prod : prod - base);

        /* compare diff / y  with  best_diff / best_y
           by cross‑multiplication to avoid division.         */
        const unsigned __int128 left  = diff  * (unsigned __int128)best_y;
        const unsigned __int128 right = best_diff * (unsigned __int128)y;

        if (left < right)
        {
            best_y   = (seq_t)y;
            best_diff = diff;
        }
    }

    return best_y;
}
