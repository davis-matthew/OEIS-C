/*
 * A269246  Number of times the digit 6 appears in the decimal expansion of
 *          n^3.
 * https://oeis.org/A269246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of times the digit 6 occurs in the decimal *
 *            representation of n^3
 * Range:     u8 n <= 15, u16 n <= 40, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t cube = n_seq * n_seq * n_seq;
    unsigned char cnt[10] = {0};
    digit_counts(cube, cnt);
    return (seq_t)cnt[6];
}
