/*
 * A369860  The orbit of n under iterations of x -> c(x)*10^L(x-c(x)) +
 *          x-c(x), where c(x) = floor(x^(1/3))^3, L(x) =
 *          floor(log_10(max(x,1))+1), enters a pseudo-loop x(k) = a^3 *
 *          10^((k-k0)*L(b)) + b beyond some k0. This sequence lists the
 *          a-values.
 * https://oeis.org/A369860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the base cube root appearing in the pseudo‑loop of the
 *            iteration x -> concat(c(x), x-c(x)), where c(x) =
 *            floor(x^(1/3))^3 and L(b) = number of decimal digits of b
 * Range:     u8 none, u16 n <= 17, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   digit_len, floor_cube_root, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_cube_root.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          // current value in the orbit
    for (;;)
    {
        seq_t rc   = floor_cube_root(x);          // floor cube root of current value
        seq_t c    = rc * rc * rc;                // largest perfect cube ≤ x
        seq_t b    = x - c;                       // cube excess
        int   d    = digit_len(b);                // number of decimal digits of b

        /* When d is a multiple of 3 the orbit has entered the
           pseudo‑loop; the base cube root a is rc with trailing zeros removed. */
        if (d % 3 == 0)
        {
            while (rc % 10 == 0)
                rc /= 10;
            return rc;
        }

        /* Otherwise, compute the next term of the orbit:
           next = c * 10^d + b. */
        seq_t pow10d = pow10((idx_t)d);
        x = c * pow10d + b;
    }
}
