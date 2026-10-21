/*
 * A269247  Number of times the digit 7 appears in the decimal expansion of
 *          n^3.
 * https://oeis.org/A269247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   count of digit '7' in the decimal expansion of n^3
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   digit_counts, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_counts.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute n^3 */
    seq_t cube = seq_pow((seq_t)n, 3);

    /* Count digits of cube */
    unsigned char cnt[10] = {0};
    digit_counts(cube, cnt);

    /* Return count of digit '7' */
    return (seq_t)cnt[7];
}
