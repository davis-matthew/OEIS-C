/*
 * A301621  Numbers not divisible by 2, 3 or 5 (A007775) with digital root 2.
 * https://oeis.org/A301621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 90 * ((n-1)/4) + [11,29,47,83][(n-1)%4]
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t _residues[4] = {11, 29, 47, 83};

seq_t a(idx_t n) {
    /* the sequence is defined for n >= 1; behaviour for n == 0 is undefined */
    idx_t k = (n - 1) / 4;          /* number of complete blocks of 4 */
    idx_t r = (n - 1) % 4;          /* position inside the block */
    seq_t term = 90 * (seq_t)k + _residues[r];
    return term;
}
