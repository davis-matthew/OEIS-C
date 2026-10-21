/*
 * A269244  Number of times the digit 4 appears in the decimal expansion of
 *          n^3.
 * https://oeis.org/A269244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 1875, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of times the digit 4 appears in the decimal expansion of n^3. */
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t cube = ns * ns;
    cube = cube * ns;                    /* cube = n^3 */
    unsigned char cnt[10];
    digit_counts(cube, cnt);             /* cnt[0]..cnt[9] hold digit frequencies */
    return (seq_t)cnt[4];                /* number of '4's */
}
