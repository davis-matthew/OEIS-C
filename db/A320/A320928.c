/*
 * A320928  Positions of 0 in A320927.
 * https://oeis.org/A320928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A320927 (A320927 is the
 *            concatenation of the iterates of 0 -> {0,0,1}, 1 -> {0,1,0},
 *            starting from 0); computed by a linear scan
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9798 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A320928_seg_val(idx_t k, idx_t pos)
{
    if (k == 0)
        return 0;
    int parent = A320928_seg_val(k - 1, pos / 3);
    idx_t c = pos % 3;
    return parent == 0 ? (c == 2 ? 1 : 0) : (c == 1 ? 1 : 0);
}

static int A320928_term(idx_t n)
{
    idx_t cumulative = 0, length = 1, k = 0;
    while (cumulative + length < n) {
        cumulative += length;
        length *= 3;
        k++;
    }
    return A320928_seg_val(k, n - cumulative - 1);
}

seq_t a(idx_t n)
{
    idx_t count = 0, m = 0;
    while (count < n) {
        m++;
        if (A320928_term(m) == 0)
            count++;
    }
    return (seq_t)m;
}
