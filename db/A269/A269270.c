/*
 * A269270  Number of n X 2 0..3 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling three
 *          exactly once.
 * https://oeis.org/A269270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9^(n-1)*(8*n-4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A269270, offset 1. */

#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 0; /* undefined for n<1, handle gracefully */

    /* Compute 9^(n-1) */
    seq_t power = 1;
    idx_t i;
    for (i = 1; i < n; ++i) {
        power = seq_mul(power, (seq_t)9);
    }

    /* Compute 8*n - 4 */
    seq_t factor = (seq_t)8 * (seq_t)n - (seq_t)4;

    return seq_mul(power, factor);
}
