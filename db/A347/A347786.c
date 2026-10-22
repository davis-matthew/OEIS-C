/*
 * A347786  Number of compositions (ordered partitions) of n into at most 5
 *          powers of 2.
 * https://oeis.org/A347786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into at most 5 powers of 2
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static arrays to memoise the DP for the largest n requested. */
static seq_t *res          = NULL;  /* sum of compositions with ≤5 parts */
static seq_t *dp           = NULL;  /* compositions with exactly m parts (current m) */
static seq_t *next_m       = NULL;  /* compositions with exactly m+1 parts (temporary) */
static idx_t  buf_cap      = 0;     /* current capacity of the arrays */

/* Ensure the static arrays can hold indices 0..n. */
static void ensure_arrays(idx_t n)
{
    idx_t new_cap = n + 1;                 /* we need indices 0..n inclusive */
    if (new_cap <= buf_cap)
        return;                          /* already large enough */

    size_t sz = (size_t)new_cap * sizeof(seq_t);

    seq_t *tmp = (seq_t *)realloc(res,    sz);
    if (!tmp) exit(1);
    res = tmp;

    tmp = (seq_t *)realloc(dp,     sz);
    if (!tmp) exit(1);
    dp = tmp;

    tmp = (seq_t *)realloc(next_m, sz);
    if (!tmp) exit(1);
    next_m = tmp;

    /* initialise new elements to zero */
    for (idx_t i = buf_cap; i < new_cap; ++i) {
        res[i] = 0;
        dp[i]  = 0;
        next_m[i] = 0;
    }

    buf_cap = new_cap;
}

/* Return a(n). */
seq_t a(idx_t n)
{
    ensure_arrays(n);

    /* reset arrays up to n */
    for (idx_t i = 0; i <= n; ++i) {
        res[i]     = 0;
        dp[i]      = 0;
        next_m[i]  = 0;
    }

    res[0] = 1;            /* empty composition for n = 0 */
    dp[0]  = 1;            /* one composition of 0 with 0 parts */

    /* iterate for m = 1 .. 5 parts */
    for (int m = 1; m <= 5; ++m) {
        /* compute next_m[i] = #compositions of i with exactly m parts */
        for (idx_t power = 1; power <= n; power <<= 1) {   /* powers of 2 */
            for (idx_t i = power; i <= n; ++i) {
                next_m[i] += dp[i - power];
            }
        }

        /* add compositions of length m to the total (≤5 parts) */
        for (idx_t i = 0; i <= n; ++i) {
            res[i] += next_m[i];
        }

        /* prepare for next iteration */
        seq_t *tmp = dp;
        dp = next_m;
        next_m = tmp;

        /* clear the array that will become the new next_m in the next loop */
        for (idx_t i = 0; i <= n; ++i) {
            next_m[i] = 0;
        }
    }

    return res[n];
}
