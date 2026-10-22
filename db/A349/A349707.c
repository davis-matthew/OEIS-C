/*
 * A349707  Numbers that are congruent to {0, 1, 4, 6, 8, 10, 11} (mod 12).
 * https://oeis.org/A349707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = residues[(n-1)%7] + 12*floor((n-1)/7)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers that are congruent to {0,1,4,6,8,10,11} (mod 12) */
#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* Undefined for offset, safe fallback */
    static const uint8_t residues[7] = {0,1,4,6,8,10,11};
    idx_t nm1 = n - 1;
    idx_t m   = nm1 / 7;
    idx_t k   = nm1 % 7;
    seq_t res = (seq_t)residues[k];
    seq_t add = ((seq_t)m) * (seq_t)12;
    return res + add;
}
