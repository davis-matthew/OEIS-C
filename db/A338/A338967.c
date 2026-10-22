/*
 * A338967  Number of achiral colorings of the 120 dodecahedral facets of the
 *          4-D 120-cell (or 120 vertices of the 4-D 600-cell) using subsets
 *          of a set of n colors.
 * https://oeis.org/A338967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (24*n^17 + 24*n^19 + 20*n^23 + 20*n^27 + 30*n^31 + n^61 +
 *            n^75)/120
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 3,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Convert n to the arbitrary‑precision type */
    seq_t sn = (seq_t)n;

    /* Compute the required powers */
    seq_t n17 = big_pow(sn, (seq_t)17);
    seq_t n19 = big_pow(sn, (seq_t)19);
    seq_t n23 = big_pow(sn, (seq_t)23);
    seq_t n27 = big_pow(sn, (seq_t)27);
    seq_t n31 = big_pow(sn, (seq_t)31);
    seq_t n61 = big_pow(sn, (seq_t)61);
    seq_t n75 = big_pow(sn, (seq_t)75);

    /* Assemble the numerator */
    seq_t num = (seq_t)24 * n17
              + (seq_t)24 * n19
              + (seq_t)20 * n23
              + (seq_t)20 * n27
              + (seq_t)30 * n31
              + n61
              + n75;

    /* Divide by 120 (exact division) */
    return num / (seq_t)120;
}
