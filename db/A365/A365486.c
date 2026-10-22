/*
 * A365486  a(n) is equal to the number of black 1 X 1 X 1 cubes in a certain
 *          coloring of the n X n X n cube (see comments for precise
 *          definition).
 * https://oeis.org/A365486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3/2 if n mod 4 = 0 * (n^3 - 3n)/2 + 2 if n mod 4 = 1 *
 *            n^3/2 + 4 if n mod 4 = 2 * (n^3 - 3n)/2 - 2 if n mod 4 = 3 *
 *            (The formula is valid for n ≥ 1; n = 1 is a special case to
 *            avoid * unsigned underflow in the subtraction.)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the small case where n^3 - 3n would underflow an
       unsigned value.  For n = 1 the correct term is 1. */
    if (n == 1)
        return (seq_t)1;

    /* Promote to seq_t and compute intermediate values. */
    seq_t nn      = (seq_t)n;
    seq_t n3      = nn * nn * nn;      /* n³ */
    seq_t three_n = (seq_t)3 * nn;

    /* Reduce n modulo 4 (fast bitwise) */
    idx_t r = n & 3;

    switch (r) {
        case 0:  /* n ≡ 0 (mod 4) */
            return n3 / 2;
        case 1:  /* n ≡ 1 (mod 4) */
            return (n3 - three_n) / 2 + (seq_t)2;
        case 2:  /* n ≡ 2 (mod 4) */
            return n3 / 2 + (seq_t)4;
        case 3:  /* n ≡ 3 (mod 4) */
            return (n3 - three_n) / 2 - (seq_t)2;
    }
    return (seq_t)0;  /* unreachable, but satisfies compiler */
}
