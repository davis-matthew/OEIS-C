/*
 * A266314  Least positive integer x such that n + x^7 = y^2 + z^2 for some
 *          positive integers y and z, or 0 if no such x exists.
 * https://oeis.org/A266314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 54, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the least positive integer x such that
 * n + x^7 equals the sum of two non‑zero squares.  If no such x
 * exists, it returns 0.  The implementation searches x starting
 * at 1 and checks for a representation by enumerating possible
 * y values.  It stops when a representation is found or when
 * x exceeds 10000 (in which case 0 is returned). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"

/* seq_t and idx_t types are provided by the build environment. */
seq_t a(idx_t n)
{
    /* Outer loop over candidate x */
    for (idx_t x = 1; x <= 10000; ++x) {
        /* m = n + x^7, using arbitrary‑precision arithmetic */
        seq_t powx = big_pow((seq_t)x, (seq_t)7);
        seq_t m = (seq_t)n + powx;

        /* Attempt to find positive y,z such that y^2 + z^2 = m */
        if (m <= 1) {
            continue; /* m < 2 cannot be expressed as sum of two positive squares */
        }

        /* Max possible y is floor(sqrt(m-1)) because z>0 */
        seq_t max_y = floor_sqrt(m - (seq_t)1);
        seq_t y_sq = (seq_t)1;          /* y = 1 -> y^2 = 1 */
        for (seq_t y = 1; y <= max_y; ++y) {
            seq_t z2 = m - y_sq;        /* Remaining part for z^2 */
            if (z2 != 0) {              /* Ensure z > 0 */
                seq_t z = floor_sqrt(z2);
                if (z * z == z2) {
                    return (seq_t)x;    /* Found minimal x */
                }
            }
            /* Prepare y_sq for next iteration: (y+1)^2 = y^2 + 2*y + 1 */
            y_sq += 2 * y + (seq_t)1;
        }
    }
    return (seq_t)0;  /* No suitable x found within the search bound */
}
