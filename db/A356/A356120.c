/*
 * A356120  Irregular triangle read by rows: the n-th row lists the values
 *          0..2^n-1 representing all subsets of a set of n elements. When its
 *          elements are linearly ordered, the subsets are lexicographically
 *          ordered.
 * https://oeis.org/A356120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) computed by recursively mapping index n into the
 *            lexicographic subset order defined by the OEIS entry for A356120
 * Range:     u8 n <= 511, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_log2_idx(idx_t v) {
    /* v > 0 */
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

static inline seq_t row_value(idx_t row, idx_t idx) {
    if (row == 0 || idx == 0)
        return 0;
    idx_t half = ((idx_t)1 << (row - 1));
    if (idx <= half)
        return (seq_t)half + row_value(row - 1, idx - 1);
    else
        return row_value(row - 1, idx - half);
}

seq_t a(idx_t n) {
    idx_t v = n + 1;
    idx_t m = floor_log2_idx(v);
    idx_t pow2m = ((idx_t)1 << m);
    idx_t row_start = pow2m - 1;
    idx_t k = n - row_start;
    return row_value(m, k);
}
