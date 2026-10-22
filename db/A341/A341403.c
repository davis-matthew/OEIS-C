/*
 * A341403  Number of nonnegative solutions to (x_1)^2 + (x_2)^2 + ... +
 *          (x_8)^2 <= n.
 * https://oeis.org/A341403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integer 8‑tuples (x1,…,x8) such
 *            that x1^2 + x2^2 + … + x8^2 ≤ n (OEIS A341403). Computation uses
 *            dynamic programming over the 8 dimensions and a convolution with
 *            the set of squares up to √n
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static size_t _int_sqrt(idx_t n) {
    /* integer floor of sqrt(n) for n ≤ UINT64_MAX,  n fits in idx_t */
    size_t r = 0;
    while ((idx_t)(r + 1) * (idx_t)(r + 1) <= n)
        ++r;
    return r;
}

seq_t a(idx_t n) {
    const int DIM = 8;                     /* fixed number of variables */
    if (n == (idx_t)(-1)) return 0;        /* just to silence unused warning */

    /* build list of squares 0^2 … floor(√n)^2 */
    size_t max_k = _int_sqrt(n);
    size_t sq_cnt = max_k + 1;             /* include 0 */
    seq_t *squares = (seq_t*)malloc(sq_cnt * sizeof(seq_t));
    if (!squares) return 0;                /* allocation failure -> 0 */
    squares[0] = 0;
    for (size_t i = 1; i <= max_k; ++i)
        squares[i] = (seq_t)i * (seq_t)i;

    /* allocate DP arrays (prev and curr) */
    seq_t *prev = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *curr = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!prev || !curr) { free(squares); if (!prev) free(prev); if (!curr) free(curr); return 0; }

    /* initial state for 0 dimensions: one tuple with sum 0 */
    prev[0] = 1;

    for (int d = 0; d < DIM; ++d) {
        /* zero the current array */
        for (idx_t i = 0; i <= n; ++i) curr[i] = 0;

        for (idx_t i = 0; i <= n; ++i) {
            seq_t val = prev[i];
            if (!val) continue;
            for (size_t s_i = 0; s_i < sq_cnt; ++s_i) {
                seq_t s = squares[s_i];
                idx_t sum = i + (idx_t)s;
                if (sum > n) break;           /* squares are increasing */
                curr[sum] += val;
            }
        }
        /* swap prev and curr */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* sum counts for all sums ≤ n to get the required value */
    seq_t total = 0;
    for (idx_t i = 0; i <= n; ++i) total += prev[i];

    /* cleanup */
    free(squares);
    free(prev);
    free(curr);

    return total;
}
