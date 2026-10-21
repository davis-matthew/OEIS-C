/*
 * A261190  Leap years in Symmetry454 calendar, starting year AD 1.
 * https://oeis.org/A261190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest y ≥ 1 such that ((52*y + 146) % 293) < 52 and
 *            gives the n‑th such y. (i.e. a(n) is the n‑th leap year.)
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* We count leap years directly; density ≈ 52/293, so for
       n ≤ 10 000 the year we return is < 56 000 – a very fast loop. */
    idx_t year   = 0;          /* current year being tested */
    idx_t count  = 0;          /* number of leap years found so far */

    while (count < n) {
        year++;
        /* Compute ((52*year + 146) % 293) safely in seq_t. */
        seq_t val = ((seq_t)52 * (seq_t)year + 146) % 293;
        if (val < 52) {
            count++;
        }
    }

    return (seq_t)year;
}
