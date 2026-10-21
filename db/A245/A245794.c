/*
 * A245794  Number of preferential arrangements of n labeled elements when at
 *          least k=9 elements per rank are required.
 * https://oeis.org/A245794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 26, u64 n <= 35, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  A245794 – Number of preferential arrangements of n labeled elements
 *  when at least k = 9 elements are required in each rank.
 *
 *  Recurrence:
 *      a(0) = 1
 *      a(n) = 0                      for 1 ≤ n < k
 *      a(n) = Σ_{j=k}^{n}  C(n,j) * a(n-j)  for n ≥ k
 *  The sum counts the ways to choose j elements for the first rank
 *  (C(n,j) choices) and then arrange the remaining n-j elements.
 *
 *  Implementation uses memoised dynamic programming with the exact
 *  multiplicative binomial coefficient to keep intermediate values
 *  minimal and to avoid overflow when possible.
 */

#include <stdlib.h>

#define K 9ULL   /* Minimum elements per rank */

/* Compute binomial coefficient C(n, r) as a seq_t.
 * Uses a multiplicative formula that performs exact division at each step. */
static inline seq_t binom_n_r(idx_t n, idx_t r)
{
    if (r > n) return 0;
    if (r == 0 || r == n) return 1;
    if (r > n - r) r = n - r;          /* use symmetry to reduce loops */
    seq_t res = 1;
    for (idx_t i = 1; i <= r; ++i) {
        /* res = res * (n - r + i) / i   — division is exact */
        res = (res * (seq_t)(n - r + i)) / (seq_t)i;
    }
    return res;
}

/* Memoisation table: dynamically grows to accommodate requested indices. */
static seq_t *memo = NULL;
static idx_t memo_len = 0;

/* Return a(n).  Works for any n ≥ 0; memoised for efficiency. */
seq_t a(idx_t n)
{
    if (n >= memo_len) {
        /* Allocate space up to n inclusive. */
        size_t new_len = (size_t)n + 1;
        seq_t *new_mem = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
        if (!new_mem) abort();          /* allocation error */
        memo = new_mem;

        /* Compute new entries sequentially. */
        for (idx_t i = memo_len; i <= n; ++i) {
            if (i == 0) {
                memo[i] = 1;
            } else if (i < K) {
                memo[i] = 0;
            } else {
                seq_t bin = binom_n_r(i, K);   /* C(i, K) */
                seq_t total = 0;
                for (idx_t j = K; j <= i; ++j) {
                    total += bin * memo[i - j];
                    if (j < i) {
                        /* C(i, j+1) = C(i, j) * (i-j) / (j+1) */
                        bin = (bin * (seq_t)(i - j)) / (seq_t)(j + 1);
                    }
                }
                memo[i] = total;
            }
        }
        memo_len = n + 1;
    }
    return memo[n];
}
