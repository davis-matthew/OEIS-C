/*
 * A307230  Number of divisible pairs of distinct positive integers up to n
 *          with at least one binary carry.
 * https://oeis.org/A307230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (i,j) with 1 ≤ i < j ≤ n, * i divides j,
 *            and (i & j) != 0. * Computation: iterate over i and its
 *            multiples j = i*m (m ≥ 2) * and test whether i and j share a
 *            1–bit
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;
        for (idx_t m = 2; ; ++m) {
            idx_t j = i * m;
            if (j > n) break;
            seq_t j_seq = (seq_t)j;
            if ((i_seq & j_seq) != (seq_t)0) {
                ++count;
            }
        }
    }
    return count;
}
