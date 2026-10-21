/*
 * A256317  Number of partitions of 4n into exactly 6 parts.
 * https://oeis.org/A256317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 203, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A256317: Number of partitions of 4n into exactly 6 parts.
   Computation uses the recurrence
       p_k(n) = p_{k-1}(n-1) + p_k(n-k)   for n >= k > 0,
   with p_0(0)=1 and p_k(n)=0 if k>n or k<0. */
/* The function memoises a 2‑D table of dp[i][j] (i <= 4*n, j <= 6). */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define PARTS 6

static seq_t *dp = NULL;          /* flattened array dp[i*(PARTS+1)+j] */
static idx_t  cap = 0;            /* allocated maximum i value + 1 */

/* Ensure dp has room for indices 0 .. max_i (inclusive). */
static void ensure_cache(idx_t max_i)
{
    if (max_i + 1 <= cap)
        return;

    idx_t old_cap = cap;
    idx_t new_cap = (cap * 2 > max_i + 1) ? cap * 2 : max_i + 1;
    if (new_cap == 0)
        new_cap = 1;

    dp = (seq_t *)realloc(dp, new_cap * (PARTS + 1) * sizeof(seq_t));
    if (!dp) abort(); /* memory exhaustion */

    /* Initialise new entries, starting at i = old_cap */
    for (idx_t i = old_cap; i < new_cap; ++i) {
        size_t base = (size_t)i * (PARTS + 1);
        for (int j = 0; j <= PARTS; ++j)
            dp[base + j] = 0;
        dp[base + 0] = (i == 0) ? 1 : 0;   /* dp[0][0] = 1, others 0 */
        for (int j = 1; j <= PARTS; ++j) {
            if (i < (idx_t)j) {
                dp[base + j] = 0;
                continue;
            }
            seq_t val = dp[(size_t)(i - 1) * (PARTS + 1) + (j - 1)];
            val += dp[(size_t)(i - j) * (PARTS + 1) + j];
            dp[base + j] = val;
        }
    }
    cap = new_cap;
}

seq_t a(idx_t n)
{
    idx_t m = 4 * n;              /* we need partitions of N = 4n */
    ensure_cache(m);
    return dp[(size_t)m * (PARTS + 1) + PARTS];
}
