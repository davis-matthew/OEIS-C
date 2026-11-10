/*
 * A242289  Number of terms in the greedy sum for the n-th tetrahedral number.
 * https://oeis.org/A242289
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of earlier tetrahedral numbers T_j = j(j+1)(j+2)/6
 *            (j = n-1 down to 1) subtracted, each at most once and taken
 *            greedily from the largest, starting from T_n; computed by direct
 *            simulation
 * Range:     u8 n <= 10, u16 n <= 75, u32 n <= 1743, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 102 terms through n = 1921 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    seq_t rem = (seq_t)n * ((seq_t)n + 1) * ((seq_t)n + 2) / 6;
    seq_t cnt = 0;

    for (idx_t j = n - 1; j > 0; --j) {
        seq_t coin = (seq_t)j * ((seq_t)j + 1) * ((seq_t)j + 2) / 6;
        if (coin <= rem) {
            rem -= coin;
            cnt++;
        }
    }
    return cnt;
}
