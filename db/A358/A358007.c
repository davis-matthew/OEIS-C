/*
 * A358007  Number of partitions of n into 7 distinct positive Fibonacci
 *          numbers (with a single type of 1).
 * https://oeis.org/A358007
 *
 * Offset:    53    (a(53) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into 7 distinct positive Fibonacci
 *            numbers (using only one '1') a(n) counts the ways to choose 7
 *            distinct Fibonacci numbers (1,2,3,5,8,…) whose sum equals n. For
 *            n < 53 the answer is 0
 * Range:     u8 n <= 140, u16 n <= 140, u32 n <= 140, u64 n <= 140,
 *            u128 n <= 140, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *dp = NULL;          /* DP table, linearized dp[j][s] */
static size_t dp_rows = 0;        /* number of rows stored (==8) */
static size_t dp_cols = 0;        /* maximum n+1 allocated */

/* ensure dp has space for at least rows * (cols) entries */
static void alloc_dp(size_t cols_needed)
{
    size_t required = (size_t)8 * (cols_needed + 1);
    if (dp_cols >= cols_needed + 1)
        return;            /* already large enough */

    seq_t *new_dp = (seq_t *)realloc(dp, required * sizeof(seq_t));
    if (!new_dp)
        return;            /* allocation failure – leave dp NULL */

    dp = new_dp;
    dp_cols = cols_needed + 1;
    dp_rows = 8;

    /* clear all entries */
    for (size_t i = 0; i < required; ++i)
        dp[i] = 0;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 53)
        return 0;

    /* generate Fibonacci numbers <= n (without duplicate 1) */
    seq_t fib[64];  /* plenty for any reasonable n */
    idx_t fib_cnt = 0;
    fib[0] = 1;
    fib[1] = 2;
    fib_cnt = 2;
    while (1) {
        seq_t nxt = fib[fib_cnt - 1] + fib[fib_cnt - 2];
        if (nxt > n)
            break;
        fib[fib_cnt++] = nxt;
    }

    size_t N = (size_t)n;
    alloc_dp(N);
    if (!dp)
        return 0;   /* allocation failed, return 0 */

    /* DP: dp[j][s] = number of ways to reach sum s using j numbers */
    dp[0] = 1;  /* one way to make sum 0 with 0 numbers */

    for (idx_t fi = 0; fi < fib_cnt; ++fi) {
        seq_t f = fib[fi];
        size_t fsize = (size_t)f;
        for (int j = 7; j >= 1; --j) {
            seq_t *rowj   = dp + j * (N + 1);
            seq_t *rowj_1 = dp + (j - 1) * (N + 1);
            for (size_t s = N; s >= fsize; --s) {
                rowj[s] += rowj_1[s - fsize];
                if (s == fsize)
                    break;  /* avoid wrap‑around of unsigned s */
            }
        }
    }

    return dp[7 * (N + 1) + N];
}
