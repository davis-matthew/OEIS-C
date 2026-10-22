/*
 * A341065  Number of ways to write n as an ordered sum of 5 squarefree
 *          numbers.
 * https://oeis.org/A341065
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ( ∑_{k≥1, k squarefree} x^k )^5
 * Range:     u8 n <= 13, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Simple square‑free test by checking squares of all integers up to sqrt(x). */
static bool is_squarefree_uint64(uint64_t x)
{
    if (x < 2) return true;          /* 1 is squarefree by convention */
    for (uint64_t p = 2; p * p <= x; ++p) {
        uint64_t pp = p * p;
        if (x % pp == 0) return false;
    }
    return true;
}

/* Cached sequence values up to the largest n seen so far. */
static seq_t *a_cache = NULL;    /* will hold a(0)…a(Nmax) */
static idx_t  cache_cap = 0;     /* number of allocated elements (Nmax+1) */

seq_t a(idx_t n)
{
    if (n < 5) return 0;               /* offset starts at 5 */

    /* Recompute the whole cache if we need a new maximum. */
    if (n >= cache_cap) {
        /* Allocate new cache. */
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t*)calloc((size_t)new_cap, sizeof(seq_t));
        if (!new_cache) return 0;      /* allocation failed */

        /* Free any previous cache. */
        free(a_cache);
        a_cache = new_cache;
        cache_cap = new_cap;

        /* Build list of square‑free numbers up to n. */
        int *sq = (int*)malloc((size_t)n * sizeof(int));
        size_t sq_sz = 0;
        for (idx_t i = 1; i <= n; ++i) {
            if (is_squarefree_uint64((uint64_t)i))
                sq[sq_sz++] = (int)i;
        }

        /* DP arrays for t‑length compositions.  We only keep two rows. */
        size_t len = (size_t)n + 1;
        seq_t *prev = (seq_t*)calloc(len, sizeof(seq_t));
        seq_t *curr = (seq_t*)calloc(len, sizeof(seq_t));
        if (!prev || !curr) {
            free(sq);
            free(prev);
            free(curr);
            return 0;
        }

        prev[0] = 1;   /* base case: one way to sum to 0 with 0 numbers */

        for (int t = 1; t <= 5; ++t) {
            /* clear current row */
            for (size_t i = 0; i < len; ++i) curr[i] = 0;

            /* convolution with the square‑free set */
            for (size_t idx = 0; idx < sq_sz; ++idx) {
                int k = sq[idx];
                for (idx_t i = k; i <= n; ++i) {
                    curr[i] += prev[i - k];
                }
            }
            /* swap rows for next iteration */
            seq_t *tmp = prev;
            prev = curr;
            curr = tmp;
        }

        /* prev now holds dp[5][i] for all i.  Store them in the cache. */
        for (idx_t i = 0; i <= n; ++i) {
            a_cache[i] = prev[i];
        }

        /* Clean up. */
        free(prev);
        free(curr);
        free(sq);
    }

    return a_cache[n];
}
