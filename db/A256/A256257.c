/*
 * A256257  6 times numbers of Gould's sequence A001316.
 * https://oeis.org/A256257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 6 * 2^{popcount(n)} where popcount(n) is the number of
 *            1‑bits in n
 * Range:     u8 n <= 62, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for idx_t and seq_t types, if needed */

seq_t a(idx_t n)
{
    /* count the 1‑bits of n without using any external utility */
    unsigned pop = 0;
    idx_t t = n;
    while (t) {
        t &= t - 1;   /* clears the lowest-set bit */
        pop++;
    }

    /* compute 2^pop as a seq_t, avoiding undefined shift on large widths */
    seq_t power = 1;
    for (unsigned i = 0; i < pop; ++i) {
        power *= 2;
    }

    /* multiply by 6 to obtain the final sequence value */
    return power * 6;
}
