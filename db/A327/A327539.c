/*
 * A327539  Starting from n: as long as the decimal representation starts with
 *          a positive even number, divide the largest such prefix by 2; a(n)
 *          corresponds to the final value.
 * https://oeis.org/A327539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 309, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A327539.  For a given n (index), repeatedly split its
   decimal representation into the largest possible even prefix u
   and an odd suffix v.  Replace u by u/2 (integer division) and
   concatenate with v.  The process stops when no even prefix
   remains; the resulting value is a(n).  This directly implements
   the OEIS definition without any lookup tables.  */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    if (m == 0)
        return 0;

    for (;;) {
        seq_t pow10 = 1;               /* 10^t, t = suffix length        */
        bool found = false;
        seq_t suffix = 0;
        seq_t new_prefix = 0;

        /* Search for the smallest pow10 (largest even prefix)
           such that the prefix m/pow10 is even.  */
        while (pow10 <= m) {
            seq_t prefix = m / pow10;
            if ((prefix & 1) == 0) {    /* even prefix found */
                found = true;
                suffix = m % pow10;
                new_prefix = prefix / 2;
                break;
            }
            /* Prepare next pow10 = pow10 * 10 without overflow.  */
            if (pow10 > m / 10)   /* next pow10 would exceed m or overflow */
                break;
            pow10 *= 10;
        }

        if (!found)                     /* no even prefix – terminate   */
            break;

        /* Build the new number: (u/2) followed by v. */
        m = new_prefix * pow10 + suffix;
    }

    return m;
}
