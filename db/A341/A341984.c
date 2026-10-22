/*
 * A341984  Number of ways to write n as an ordered sum of 5 primes (counting
 *          1 as a prime).
 * https://oeis.org/A341984
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered 5‑tuples of primes (1 treated as a
 *            prime) whose sum equals n. The values are computed by dynamic
 *            programming using the generating function (x + Σ x^prime)^5
 * Range:     u8 n <= 16, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Helper: generate all primes ≤ limit (using simple sieve).             */
/* The function allocates an array of uint64_t and stores its address   */
/* in *primes, and the number of primes in *cnt. It is intended for     */
/* small limits (≤ 10 000) and is freed by the caller.                  */
/* --------------------------------------------------------------------- */
static void build_primes_upto(uint64_t limit, uint64_t **primes_out,
                              size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    /* simple sieve – memory negligible for small limit */
    uint8_t *mark = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!mark) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    memset(mark, 0, (limit + 1) * sizeof(uint8_t));

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!mark[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                mark[j] = 1;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!mark[i]) ++cnt;

    /* Allocate array of primes */
    uint64_t *pr = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!pr) {
        free(mark);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    /* Store primes */
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!mark[i]) pr[idx++] = i;

    free(mark);
    *primes_out = pr;
    *cnt_out = cnt;
}

/* --------------------------------------------------------------------- */
/* Global cache for computed values.  dp_cache[i] holds a(i) for i      */
/* up to cache_size.  The cache is built lazily and extended on demand.  */
/* --------------------------------------------------------------------- */
static seq_t *dp_cache = NULL;
static idx_t   cache_size = 0;

/* Rebuild the cache up to 'upto' (inclusive).  This function assumes   */
/* that no previous cache exists or that the old cache is discarded.    */
/* --------------------------------------------------------------------- */
static void recompute_cache(idx_t upto)
{
    size_t sz = (size_t)upto + 1;

    /* Allocate two working arrays of seq_t */
    seq_t *cur  = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *next = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!cur || !next) {
        free(cur); free(next);
        return;
    }

    /* Initialise to zero */
    memset(cur, 0, sz * sizeof(seq_t));
    memset(next, 0, sz * sizeof(seq_t));
    cur[0] = (seq_t)1;          /* one way to reach sum 0 with 0 parts */

    /* Build the list of primes up to 'upto' */
    uint64_t *prime_list = NULL;
    size_t prime_cnt = 0;
    build_primes_upto((uint64_t)upto, &prime_list, &prime_cnt);

    /* Perform 5 convolutions: each iteration adds one more part. */
    for (int part = 0; part < 5; ++part) {
        /* clear next */
        memset(next, 0, sz * sizeof(seq_t));

        for (size_t sum = 0; sum < sz; ++sum) {
            seq_t val = cur[sum];
            if (!val) continue;

            /* add a part equal to 1 */
            if (sum + 1 < sz) next[sum + 1] += val;

            /* add parts that are primes */
            for (size_t k = 0; k < prime_cnt; ++k) {
                size_t new_sum = sum + (size_t)prime_list[k];
                if (new_sum >= sz) break;
                next[new_sum] += val;
            }
        }
        /* swap cur and next */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* cur now contains the final counts.  Replace the global cache. */
    if (dp_cache) free(dp_cache);   /* discard old cache */
    dp_cache = cur;                 /* keep pointer to the finished array */
    free(next);                     /* free unused temporary array */
    free(prime_list);               /* clean up prime list */
    cache_size = upto;
}

/* --------------------------------------------------------------------- */
/* Public function required by the spec.  Returns a(n) for n ≥ 5.       */
/* In all other cases it returns 0.                                       */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < (idx_t)5) return (seq_t)0;          /* defined only for n ≥ 5 */

    if (cache_size < n) recompute_cache(n);     /* extend cache if needed */

    return dp_cache[n];
}
