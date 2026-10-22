/*
 * A326523  Sum of all the parts in the partitions of n into 9 squarefree
 *          parts.
 * https://oeis.org/A326523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A326522(n)
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(n) = n * (number of partitions of n into exactly 9 squarefree parts) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Integer square root for unsigned 64‑bit values. */
static inline idx_t int_sqrt(idx_t v)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= v)
        ++r;
    return r;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* ----- static state for memoisation ----- */
    static seq_t *data      = NULL;  /* flattened 10 × (maxN+1) table   */
    static seq_t **rows     = NULL;  /* row pointers 0 … 9              */
    static idx_t curMax     = 0;     /* largest n processed so far      */

    /* Rebuild tables if we are asked for a larger n or if nothing has
       been built yet.  This also handles the first call (n = 0). */
    if (data == NULL || n > curMax)
    {
        /* Free old tables if they exist.      */
        if (data) free(data);
        if (rows) free(rows);

        /* Allocate new tables.                 */
        idx_t maxN = n;
        size_t total = (size_t)(maxN + 1) * 10; /* 10 rows of size maxN+1 */
        data = (seq_t *)malloc(total * sizeof(seq_t));
        rows = (seq_t **)malloc(10 * sizeof(seq_t *));
        if (!data || !rows)    /* allocation failure */
        {
            free(data); free(rows);
            return 0;
        }

        /* Point each row into the flattened buffer.        */
        for (int k = 0; k < 10; ++k)
            rows[k] = data + (size_t)k * (maxN + 1);

        /* Initialization: dp[0][0] = 1, others 0.          */
        for (idx_t i = 0; i <= maxN; ++i)
            rows[0][i] = (i == 0) ? 1 : 0;
        for (int k = 1; k < 10; ++k)
            for (idx_t i = 0; i <= maxN; ++i)
                rows[k][i] = 0;

        /* ----- Build list of squarefree integers ≤ maxN ----- */
        bool *is_prime = (bool *)malloc((maxN + 1) * sizeof(bool));
        bool *sqfree   = (bool *)malloc((maxN + 1) * sizeof(bool));
        if (!is_prime || !sqfree)
        {
            free(is_prime); free(sqfree);
            free(data); free(rows);
            return 0;
        }

        /* Sieve of Eratosthenes.      */
        for (size_t i = 0; i <= maxN; ++i) {
            is_prime[i] = true;
            sqfree[i]   = true;
        }
        if (maxN >= 0) is_prime[0] = false;
        if (maxN >= 1) is_prime[1] = false;

        idx_t sqroot = int_sqrt(maxN);
        for (idx_t i = 2; i <= sqroot; ++i)
            if (is_prime[i])
                for (idx_t j = i * i; j <= maxN; j += i)
                    is_prime[j] = false;

        /* Remove numbers that contain a squared prime divisor. */
        for (idx_t i = 2; i <= sqroot; ++i)
            if (is_prime[i]) {
                idx_t sq = i * i;
                for (idx_t m = sq; m <= maxN; m += sq)
                    sqfree[m] = false;
            }

        /* Collect squarefree numbers into an array.          */
        int *sq_list = (int *)malloc((maxN + 1) * sizeof(int));
        if (!sq_list) {
            free(is_prime); free(sqfree);
            free(data); free(rows);
            return 0;
        }
        int sq_len = 0;
        for (idx_t i = 1; i <= maxN; ++i)
            if (sqfree[i])
                sq_list[sq_len++] = (int)i;

        /* ----- DP: compute partitions into exactly 9 parts ----- */
        for (int idx = 0; idx < sq_len; ++idx) {
            int v = sq_list[idx];
            for (int k = 1; k <= 9; ++k) {
                for (idx_t t = v; t <= maxN; ++t) {
                    rows[k][t] += rows[k-1][t - (idx_t)v];
                }
            }
        }

        /* Clean up temporary arrays.                           */
        free(is_prime);
        free(sqfree);
        free(sq_list);

        curMax = maxN;
    }

    /* The number of partitions of n into 9 squarefree parts. */
    seq_t part_cnt = rows[9][n];

    /* a(n) = n * part_cnt.  Use seq_t arithmetic for all ops. */
    return part_cnt * (seq_t)n;
}
