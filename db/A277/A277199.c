/*
 * A277199  Square array A(n,k) = A048720(n, A065621(k)), read by descending
 *          antidiagonals as A(1,1), A(1,2), A(2,1), A(1,3), A(2,2), A(3,1),
 *          etc.
 * https://oeis.org/A277199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   A(n,k) = A048720(n, A065621(k)) (carryless GF(2) product of n
 *            and n XOR ((k-lowbit(k))<<1)... applied to k), read by
 *            descending antidiagonals
 * Range:     u8 n <= 283, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Validated: standard, pass, 107 terms through n = 7119 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t
A065621_a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    idx_t low = n & (~n + 1);
    seq_t low_t = (seq_t)low;
    return n_t ^ ((n_t - low_t) << 1);
}

static seq_t
A048720_mul(idx_t i, seq_t j)
{
    seq_t result = 0;
    seq_t shifted = j;
    while (i) {
        if (i & 1)
            result ^= shifted;
        shifted <<= 1;
        i >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < n)
        d++;
    idx_t prev = ((d - 1) * d) / 2;
    idx_t row = n - prev;          /* 1..d */
    idx_t col = d + 1 - row;
    return A048720_mul(row, A065621_a(col));
}
