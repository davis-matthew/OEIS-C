/*
 * A327138  Numbers k such that cos(2k) < cos(2k+2).
 * https://oeis.org/A327138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for sequence A327138: the n‑th positive integer k such that
   cos(2k) < cos(2k+2).  This is equivalent to sin(2k+1) < 0.
   The routine scans k=1,2,… and counts how many times sin(2k+1) is
   negative.  When the count reaches n, the corresponding k is returned. */
#include <math.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;                 /* starting from 1 below */
    while (count < n)
    {
        ++k;
        long double val = sinl((long double)k * 2.0L + 1.0L);
        if (val < 0.0L)
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
    /* Should never reach here for n >= 1 */
    return (seq_t)0;
}
