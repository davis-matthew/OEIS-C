/*
 * A329780  Numbers that are either odd or are congruent to {+-2, +-8, +-12,
 *          +-14} (mod 32).
 * https://oeis.org/A329780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32·⌊(n–1)/24⌋ + r[(n–1) mod 24] where r =
 *            {1,2,3,5,7,8,9,11,12,13,14,15,17,18,19,20,21,23,24,25,27,29,30,31}
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint8_t residues[24] = {
    1, 2, 3, 5, 7, 8, 9, 11,
   12,13,14,15,17,18,19,20,
   21,23,24,25,27,29,30,31
};

seq_t a(idx_t n)
{
    /* Assumes n >= 1; behaviour for n < 1 is undefined. */
    idx_t block   = (n - 1) / 24;          /* zero‑based block index   */
    idx_t offset  = (n - 1) % 24;          /* position inside block    */
    /* 32 = 1<<5; use shift to avoid multiplication in integer types. */
    return ((seq_t)block << 5) + residues[offset];
}
