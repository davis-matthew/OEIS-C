/*
 * A246302  Numbers k such that cos(k) < cos(k+1) < cos(k+2) < cos(k+3) >
 *          cos(k+4).
 * https://oeis.org/A246302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS A246302.
   A term k satisfies
        cos(k) < cos(k+1) < cos(k+2) < cos(k+3) > cos(k+4)
   which is equivalent to
        sin(k+0.5) < 0
        sin(k+1.5) < 0
        sin(k+2.5) < 0
        sin(k+3.5) > 0
   (floating‑point computations are used only for the test; the
   seq_t value itself is produced without floating point arithmetic). */

#include <math.h>

seq_t a(idx_t n)
{
    /* The offset of the sequence is 1; for n==0 we return 0. */
    if (n == 0) return 0;

    idx_t k = 1;                  /* candidate integer k */
    idx_t found = 0;              /* number of terms found so far */

    while (found < n) {
        /* Evaluate the four trigonometric sign conditions. */
        double s0 = sin((double)k + 0.5);
        double s1 = sin((double)k + 1.5);
        double s2 = sin((double)k + 2.5);
        double s3 = sin((double)k + 3.5);

        if (s0 < 0.0 && s1 < 0.0 && s2 < 0.0 && s3 > 0.0) {
            found++;
            if (found == n)
                return (seq_t)k;
        }
        k++;
    }

    /* Unreachable for valid n. */
    return 0;
}
