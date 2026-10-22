/*
 * A304036  Number of partitions of n into at most 2 copies of 1!, 3 copies of
 *          2!, 4 copies of 3!, ... .
 * https://oeis.org/A304036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A304036: number of partitions of n into at most (j+1)
 *  copies of j! (for j >= 1).  Generating function:
 *  ∏_{j≥1} ∑_{k=0}^{j+1} x^{k*j!}
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache of the sequence values */
static seq_t *table = NULL;
static size_t table_cap = 0;

/* compute a(n) */
seq_t a(idx_t n)
{
    /* if a new larger n is requested, recompute the table */
    if ((size_t)n + 1 > table_cap) {
        size_t cap = (size_t)n + 1;
        seq_t *dp   = (seq_t *)malloc(cap * sizeof(seq_t));
        seq_t *tmp  = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!dp || !tmp) { /* insufficient memory */
            if (dp) free(dp);
            if (tmp) free(tmp);
            return 0;
        }

        /* initialise dp[0] = 1, others 0 */
        for (size_t i = 0; i < cap; ++i) dp[i] = 0;
        dp[0] = 1;

        idx_t fac = 1;   /* current factorial j! (j starts at 1) */
        idx_t j   = 1;   /* current j */

        /* bounded‑knapsack style DP */
        while (fac <= n) {
            /* limit = j+1 copies of j! */
            idx_t limit = j + 1;

            /* clear temporary array */
            for (size_t i = 0; i < cap; ++i) tmp[i] = 0;

            /* add contributions */
            for (idx_t i = 0; i <= n; ++i) {
                seq_t val = dp[(size_t)i];
                if (val == 0) continue;
                for (idx_t k = 0; k <= limit; ++k) {
                    idx_t idx = i + fac * k;
                    if (idx > n) break;
                    tmp[(size_t)idx] += val;
                }
            }

            /* swap dp and tmp for next iteration */
            seq_t *swap = dp;
            dp = tmp;
            tmp = swap;

            /* prepare for next factorial */
            ++j;
            if (fac > n / j) break;   /* avoid overflow */
            fac *= j;
        }

        /* discard old table if it existed */
        if (table) free(table);

        /* set updated table */
        table = dp;
        table_cap = cap;

        /* free the temporary buffer (now contains old dp) */
        free(tmp);
    }

    return table[(size_t)n];
}
