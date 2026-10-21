/*
 * A297260  Numbers whose base-6 digits have greater up-variation than
 *          down-variation; see Comments.
 * https://oeis.org/A297260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A297260: numbers whose base‑6 digits have greater up‑variation
   than down‑variation.  The value a(n) is the n‑th positive integer
   satisfying UV(n,6) > DV(n,6).  This implementation scans numbers
   sequentially, computing UV and DV for each, until it has found the
   requested term. */

#include <stdbool.h>   /* for bool */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* below the OEIS offset – undefined */

    idx_t found = 0;                 /* how many qualifying numbers found */
    seq_t candidate = 1;             /* current integer to test */

    while (1) {
        /* Compute up‑variation (uv) and down‑variation (dv)
           of candidate in base 6. */
        seq_t uv = 0;
        seq_t dv = 0;
        unsigned int prev_digit = 0;
        bool first = true;
        seq_t tmp = candidate;

        while (tmp > 0) {
            unsigned int digit = (unsigned int)(tmp % 6);
            tmp /= 6;

            if (first) {
                prev_digit = digit;
                first = false;
            } else {
                if (digit > prev_digit)
                    dv += (seq_t)(digit - prev_digit);
                else if (digit < prev_digit)
                    uv += (seq_t)(prev_digit - digit);

                prev_digit = digit;
            }
        }

        if (uv > dv) {          /* candidate qualifies for the sequence */
            ++found;
            if (found == n)      /* return the n‑th qualifying number */
                return candidate;
        }

        ++candidate;            /* test the next integer */
    }
}
