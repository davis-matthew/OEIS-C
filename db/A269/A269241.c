/*
 * A269241  Number of times the digit 1 appears in the decimal expansion of
 *          n^3.
 * https://oeis.org/A269241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of times the digit 1 appears in the decimal
 *            expansion of n³
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 2030, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t, compute the cube, count digits, and return the count of '1's. */
    seq_t nval  = (seq_t)n;
    seq_t cube  = nval * nval * nval;

    unsigned char cnt[10] = {0};
    digit_counts(cube, cnt);

    return (seq_t)cnt[1];   /* count of digit '1' */
}
