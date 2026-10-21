/*
 * A269242  Number of times the digit 2 appears in the decimal expansion of
 *          n^3.
 * https://oeis.org/A269242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of digit 2 in the decimal expansion of n^3
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 2452, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    /* Compute n^3 as a seq_t */
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;

    /* Count digits in cube */
    unsigned char cnt[10] = {0};
    digit_counts(cube, cnt);

    /* Return the count of digit '2' */
    return (seq_t)cnt[2];
}
