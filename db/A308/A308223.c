/*
 * A308223  Sum of the smallest side lengths of all integer-sided triangles
 *          with perimeter n whose side lengths are mutually coprime.
 * https://oeis.org/A308223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{triangles with perimeter n and pairwise coprime
 *            sides} of the smallest side length. TRIANGLES are considered
 *            unordered, so we iterate over a <= b <= c, compute c = n - a -
 *            b, and check the triangle inequality and coprimality
 * Range:     u8 n <= 53, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires the gcd utility:
   #include "../utils/gcd.h" */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Count all integer-sided triangles of perimeter n that are pairwise coprime
   and sum the smallest side of each. */
seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    seq_t sum = (seq_t)0;

    /* a <= b <= c, a <= n/3 */
    for (idx_t a_side = 1; a_side <= n / 3; ++a_side)
    {
        /* b must satisfy a <= b <= (n-a)/2 and a+b > n/2.
           The lower bound for b is: ceil((n+1-2*a)/2) which equals
           (n + 2 - 2*a)/2. */
        idx_t b_min = (n + 2 - 2 * a_side) / 2;
        if (b_min < a_side)
            b_min = a_side;

        /* maximum b so that c >= b: c = n - a - b >= b  =>  b <= (n-a)/2 */
        idx_t b_max = (n - a_side) / 2;

        if (b_min > b_max)
            continue;

        for (idx_t b_side = b_min; b_side <= b_max; ++b_side)
        {
            idx_t c_side = n - a_side - b_side;
            /* pairwise coprime check */
            if (gcd((seq_t)a_side, (seq_t)b_side) == (seq_t)1 &&
                gcd((seq_t)b_side, (seq_t)c_side) == (seq_t)1 &&
                gcd((seq_t)a_side, (seq_t)c_side) == (seq_t)1)
            {
                sum += (seq_t)a_side;
            }
        }
    }

    return sum;
}
