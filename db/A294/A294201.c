/*
 * A294201  Irregular triangle read by rows: T(n,k) is the number of
 *          k-partitions of {1..3n} that are invariant under a permutation
 *          consisting of n 3-cycles (1 <= k <= 3n).
 * https://oeis.org/A294201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the (n‑th) element of a triangle T(n,k) read row‑wise, *
 *            where T(n,k) satisfies * T(0,0)=1, * T(n,k) = k*T(n-1,k) +
 *            T(n-1,k-1) + T(n-1,k-3) for n>0, k>0, * and T(0,k)=0 for k>0.
 *            The sequence a(n) consists of the entries * T(1,1..3),
 *            T(2,1..6), T(3,1..9), ... (i.e. row n has 3n terms)
 * Range:     u8 n <= 33, u16 n <= 89, u32 n <= 207, u64 n <= 584,
 *            u128 n <= 1395, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n) {
    /* determine which triangle row contains the nth term */
    idx_t row = 0;
    while ( (3 * row * (row + 1)) / 2 < n ) {
        ++row;
    }
    /* compute the offset within that row */
    idx_t prev_rows = (row == 0) ? 0 : (3 * (row - 1) * row) / 2;
    idx_t k = n - prev_rows;            /* 1‑based column index within the row */

#ifdef __cplusplus
    /* C++ path – use std::vector (seq_t may be a big‑integer class) */
    std::vector<seq_t> prev(1);          /* row 0: prev[0] = 1 */
    prev[0] = 1;
    for (idx_t r = 1; r <= row; ++r) {
        size_t curr_len = 3 * r + 1;
        std::vector<seq_t> curr(curr_len, 0);
        const idx_t prev_max = 3 * (r - 1);   /* largest valid k in previous row */
        for (idx_t kk = 1; kk <= 3 * r; ++kk) {
            seq_t val = 0;
            if (kk <= prev_max) {
                val += (seq_t)kk * prev[kk];
            }
            if (kk - 1 >= 1 && kk - 1 <= prev_max) {
                val += prev[kk - 1];
            } else if (r == 1 && kk - 1 == 0) {   /* T(0,0) case */
                val += (seq_t)1;
            }
            if (kk - 3 >= 1 && kk - 3 <= prev_max) {
                val += prev[kk - 3];
            } else if (r == 1 && kk - 3 == 0) {   /* T(0,0) case for kk==3 */
                val += (seq_t)1;
            }
            curr[kk] = val;
        }
        prev.swap(curr);
    }
    return prev[(size_t)k];
#else
    /* C path – use malloc (seq_t is an unsigned integer type) */
    seq_t *prev = calloc(1, sizeof(seq_t));   /* row 0: prev[0] = 1 */
    prev[0] = 1;
    for (idx_t r = 1; r <= row; ++r) {
        size_t curr_len = 3 * r + 1;
        seq_t *curr = calloc(curr_len, sizeof(seq_t));
        const idx_t prev_max = 3 * (r - 1);
        for (idx_t kk = 1; kk <= 3 * r; ++kk) {
            seq_t val = 0;
            if (kk <= prev_max) {
                val += (seq_t)kk * prev[kk];
            }
            if (kk - 1 >= 1 && kk - 1 <= prev_max) {
                val += prev[kk - 1];
            } else if (r == 1 && kk - 1 == 0) {
                val += (seq_t)1;   /* T(0,0) */
            }
            if (kk - 3 >= 1 && kk - 3 <= prev_max) {
                val += prev[kk - 3];
            } else if (r == 1 && kk - 3 == 0) {
                val += (seq_t)1;   /* T(0,0) */
            }
            curr[(size_t)kk] = val;
        }
        free(prev);
        prev = curr;
    }
    seq_t answer = prev[(size_t)k];
    free(prev);
    return answer;
#endif
}
