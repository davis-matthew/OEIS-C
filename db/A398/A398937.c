/*
 * A398937  Nonnegative integers k satisfying cos(k) < cos(k+1) < cos(k+2) <
 *          cos(k+3) < cos(k+4) > cos(k+5).
 * https://oeis.org/A398937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the n‑th term of OEIS sequence A398937.
   The sequence consists of the nonnegative integers k such that
   cos(k) < cos(k+1) < cos(k+2) < cos(k+3) < cos(k+4) > cos(k+5).
   The implementation evaluates the trigonometric inequality by
   using the standard library's cos() routine.  Since the result
   (k) is an integer, it is cast to seq_t at the point of return.
   No mutable state or dynamic allocation is used, so the function
   behaves correctly for any build of seq_t (8–128 bit integers,
   arbitrary‑precision classes) and for any order of calls. */

#include <math.h>
#include <stdint.h>

/* return the n‑th term; n ≥ 1.  a(1) is the first element of the
   sequence (9), a(2) is the second (53), etc. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for 0, guard against caller error */

    idx_t found = 0;           /* number of terms found so far */
    for (idx_t k = 0; ; ++k)
    {
        /* Evaluate the trigonometric inequality at integer k. */
        double c0 = cos((double)k);
        double c1 = cos((double)(k + 1));
        double c2 = cos((double)(k + 2));
        double c3 = cos((double)(k + 3));
        double c4 = cos((double)(k + 4));
        double c5 = cos((double)(k + 5));

        if (c0 < c1 && c1 < c2 && c2 < c3 && c3 < c4 && c4 > c5)
        {
            ++found;
            if (found == n)          /* nth term has been found */
                return (seq_t)k;
        }
    }
}
