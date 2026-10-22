/*
 * A371571  Irregular triangle read by rows: row n lists the 1-based positions
 *          (starting from the left) of zeros in the binary expansion of n, or
 *          0 if the binary expansion of n contains only ones.
 * https://oeis.org/A371571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 8581, u16 n <= 8581, u32 n <= 8581, u64 n <= 8581,
 *            u128 n <= 8581, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A371571: for each integer r (starting at 0), write r in binary
   and output the 1–based positions of the zero bits from left to right.
   If r contains no zero bits, output a single 0.  The sequence is then
   obtained by concatenating the outputs for r = 0, 1, 2, … */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t terms = 0;        /* number of sequence values produced so far */

    for (idx_t row = 0; ; ++row)
    {
        /* Determine bit length of the current row */
        idx_t bitlen;
        if (row == 0)
            bitlen = 1;                       /* representation "0" */
        else {
            idx_t tmp = row;
            bitlen = 0;
            while (tmp) { tmp >>= 1; ++bitlen; }
        }

        /* Find zero positions in this row */
        idx_t pos[64];      /* maximal bit length for a 64‑bit integer */
        idx_t zeros = 0;
        for (idx_t i = 0; i < bitlen; ++i) {
            idx_t bit = (row == 0) ? 0
                                   : ((row >> (bitlen - 1 - i)) & 1ULL);
            if (bit == 0) {
                pos[zeros++] = i + 1;   /* 1‑based position */
            }
        }

        idx_t row_terms = (zeros == 0) ? 1 : zeros;

        /* Does the desired index n belong to this row? */
        if (n < terms + row_terms) {
            if (zeros == 0)           /* all‑ones row */
                return (seq_t)0;
            else
                return (seq_t)pos[n - terms];
        }

        terms += row_terms;
    }
}
