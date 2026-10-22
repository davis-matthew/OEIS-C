/*
 * A349702  Irregular triangle read by rows where T(n,k) is the maximum
 *          terminal Wiener index for a tree of n vertices among which k are
 *          leaves.
 * https://oeis.org/A349702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is the n‑th term of the flattened triangle T(n,k) where the
 *            triangle reads by rows: for each number of vertices r the
 *            allowed leaf counts k are: r = 0,1,2 : k = r r = 3 : k = 2 r ≥ 4
 *            : k = 2,3,…,r–1 and T(r,k) is the maximum terminal Wiener index
 *            of a tree with r vertices and k leaves, given...
 * Range:     u8 n <= 116, u16 n <= 6860, u32 n <= 7023, u64 n <= 7023,
 *            u128 n <= 7023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the trivial small terms */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* Find the row r and offset pos within that row */
    idx_t r = 0;
    idx_t idx = n;                     /* remaining index in the flattened array */
    while (1) {
        idx_t row_len = (r <= 3) ? 1 : (r - 2);
        if (idx < row_len) break;
        idx -= row_len;
        ++r;
    }
    /* idx is now the position inside row r */
    idx_t pos = idx;

    /* Determine the leaf count k for this row */
    idx_t k;
    if (r <= 3) {
        /* rows 0,1,2 use k = r, row 3 uses k = 2 */
        k = (r == 3) ? 2 : r;
    } else {
        k = 2 + pos;
    }

    /* Compute T(r,k) for r >= 3 (k >= 2) */
    seq_t r_s = (seq_t)r;
    seq_t k_s = (seq_t)k;
    seq_t floor_half = k_s / (seq_t)2;
    seq_t ceil_half  = k_s - floor_half;
    seq_t term1 = k_s * (k_s - (seq_t)1);
    seq_t term2 = (r_s - (seq_t)1 - k_s) * floor_half * ceil_half;
    return term1 + term2;
}
