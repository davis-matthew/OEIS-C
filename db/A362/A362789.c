/*
 * A362789  Triangle read by rows. T(n, k) = FallingFactorial(n - k, k) *
 *          Stirling2(n - k, k), for n >= 0 and 0 <= k <= n//2, where '//'
 *          denotes integer division.
 * https://oeis.org/A362789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r, c) where T(r, c) = FallingFactorial(r-c, c) *
 *            Stirling2(r-c, c) and r, c are the row and column positions of
 *            the flattened triangle of T. The function a(idx_t n) returns the
 *            n-th term (a(0) = 1)
 * Range:     u8 n <= 17, u16 n <= 38, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes rows of the triangle and caches the values.
   It uses a small DP routine for Stirling numbers of the second kind that
   runs in O(m*k) per call but only needs to compute values for m <= 200,
   because up to the 10 000th flattening index only about 200 rows are
   required.  */
#include <stdlib.h>

/* Fenwick factorial (falling factorial) */
static seq_t falling_factorial(idx_t m, idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 0; i < k; ++i)
        res *= (seq_t)(m - i);
    return res;
}

/* Stirling numbers of the second kind, computed with a one‑dimensional
   DP that iterates over n from 1 to m, updating from high to low k. */
static seq_t *stirling_dp = NULL;   /* buffer for DP values */
static size_t stirling_cap = 0;     /* allocated capacity of stirling_dp */

static seq_t stirling2(idx_t m, idx_t k)
{
    if (k > m) return 0;
    if (k == 0) return (m == 0) ? 1 : 0;

    if ((size_t)(k + 1) > stirling_cap) {
        size_t new_cap = (size_t)(k + 1);
        seq_t *tmp = (seq_t *)realloc(stirling_dp, new_cap * sizeof(seq_t));
        stirling_dp = tmp;
        stirling_cap = new_cap;
    }

    /* initialize dp[0 .. k] */
    for (idx_t i = 0; i <= k; ++i) stirling_dp[i] = 0;
    stirling_dp[0] = 1;   /* S(0,0) = 1 */

    for (idx_t n = 1; n <= m; ++n) {
        idx_t len = (n < k) ? n : k;   /* the current maximum k */
        for (idx_t j = len; j > 0; --j) {
            /* S(n, j) = S(n-1, j-1) + j * S(n-1, j) */
            seq_t val = stirling_dp[j-1];
            val += (seq_t)j * stirling_dp[j];
            stirling_dp[j] = val;
        }
        stirling_dp[0] = 0;   /* for n >= 1, S(n,0) = 0 */
    }
    return stirling_dp[k];
}

/* Cache for the flattened triangle */
static seq_t *seq_cache   = NULL;   /* array of computed a(n) values */
static size_t cache_cap   = 0;      /* allocated capacity of seq_cache */
static size_t cache_size  = 0;      /* number of entries actually stored */
static idx_t  next_row    = 0;      /* next row number to generate */

/* Ensure seq_cache can hold at least 'new_cap' elements */
static void ensure_cache_cap(size_t new_cap)
{
    if (new_cap > cache_cap) {
        size_t cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        if (cap < new_cap) cap = new_cap;
        seq_t *tmp = (seq_t *)realloc(seq_cache, cap * sizeof(seq_t));
        seq_cache = tmp;
        cache_cap = cap;
    }
}

/* Return the n-th term of sequence A362789 */
seq_t a(idx_t n)
{
    /* generate rows until we have at least n+1 terms */
    while (cache_size <= (size_t)n) {
        idx_t row = next_row;
        idx_t kmax = row / 2;
        for (idx_t k = 0; k <= kmax; ++k) {
            idx_t m = row - k;                 /* m = n-k */
            seq_t ff = falling_factorial(m, k);
            seq_t st = stirling2(m, k);
            seq_t val = ff * st;
            ensure_cache_cap(cache_size + 1);
            seq_cache[cache_size++] = val;
        }
        ++next_row;
    }
    return seq_cache[(size_t)n];
}
