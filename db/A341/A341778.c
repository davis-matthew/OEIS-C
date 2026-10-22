/*
 * A341778  Number of partitions of n into 7 nonzero tetrahedral numbers.
 * https://oeis.org/A341778
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 7 non‑zero tetrahedral
 *            numbers (unordered, exactly 7 parts). The function implements a
 *            standard combinatorial DP that keeps the number of parts limited
 *            to 7
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Global cache for up to the largest n requested so far. */
static seq_t *dp_cache = NULL;   /* dp[i*(N+1)+s] 0 <= i <=7, 0 <= s <= N */
static idx_t  cached_N  = 0;      /* largest N for which dp_cache is valid */

/* Recompute the DP table up to limit (inclusive).  Called only when a larger
   n is requested. */
static void compute_up_to(idx_t limit)
{
    if (limit < 7)
    {
        cached_N = limit;
        return;
    }

    /* Reallocate cache if needed */
    if (dp_cache != NULL)
        free(dp_cache);
    size_t size = (size_t)(8 * (limit + 1));
    dp_cache = (seq_t *)calloc(size, sizeof(seq_t));
    if (dp_cache == NULL)
        return;   /* allocation failed – callers could check for 0 */

    cached_N = limit;

    /* dp[0][0] = 1 */
    dp_cache[0] = 1;

    /* Pre‐compute the tetrahedral numbers <= limit */
    seq_t t_arr[200];
    size_t tcnt = 0;
    for (idx_t k = 1;; ++k)
    {
        seq_t t = (seq_t)k * (k + 1) * (k + 2) / 6;
        if (t > limit)
            break;
        t_arr[tcnt++] = t;
    }

    /* DP : iterate over each coin (tetrahedral number) */
    for (size_t idx = 0; idx < tcnt; ++idx)
    {
        seq_t t = t_arr[idx];
        idx_t t_val = (idx_t)t;
        /* For each number of parts i (1..7) */
        for (idx_t i = 1; i <= 7; ++i)
        {
            for (idx_t s = t_val; s <= limit; ++s)
            {
                dp_cache[i * (limit + 1) + s] +=
                    dp_cache[(i - 1) * (limit + 1) + (s - t_val)];
            }
        }
    }
}

/* Public interface required by the build */
seq_t a(idx_t n)
{
    if (n < 7)
        return 0;   /* offset rule */

    if (n > cached_N)
        compute_up_to(n);

    /* If compute failed, dp_cache might be NULL */
    if (dp_cache == NULL)
        return 0;

    return dp_cache[7 * (n + 1) + n];
}
