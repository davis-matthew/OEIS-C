/*
 * A369654  Positions of multiples of 3 in A327860.
 * https://oeis.org/A369654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*floor((n-1)/8) + {0,4,5,10,11,13,16,17}[(n-1)%8]
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* shift to 0‑based index */
    idx_t m = n - 1;
    idx_t q = m / 8;
    idx_t r = m % 8;
    static const seq_t pref[8] = {0, 4, 5, 10, 11, 13, 16, 17};
    return (seq_t)q * (seq_t)18 + pref[(size_t)r];
}
