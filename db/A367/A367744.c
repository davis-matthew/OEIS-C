/*
 * A367744  Expansion of e.g.f. exp(1 - x - exp(3*x)).
 * https://oeis.org/A367744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -a(n-1) - Sum_{k=1..n} binomial(n-1,k-1) * 3^k * a(n-k)
 *            Initial values: a(0) = 1 This implementation uses memoisation
 *            and an O(n²) recurrence
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 20, i128 n <= 23,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;          /* array of computed values   */
static size_t cache_cap = 0;         /* current allocated capacity */
static idx_t cache_len = 0;          /* number of values currently stored (up to cache_len-1) */

/* Compute the sequence term a(n). */
seq_t a(idx_t n)
{
    /* Ensure the cache has space for indices up to n. */
    ensure_cache(n, &cache, &cache_cap);

    /* If the requested value is not yet computed, compute it. */
    if (cache_len <= n)
    {
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i == 0)
            {
                cache[0] = 1;
                continue;
            }

            seq_t sum     = 0;
            seq_t binom   = 1;      /* binomial(n-1,0) */
            seq_t pow3    = 3;      /* 3^1 */

            for (idx_t k = 1; k <= i; ++k)
            {
                /* term = binomial(n-1,k-1) * 3^k * a(n-k) */
                seq_t term = binom * pow3 * cache[i - k];
                sum -= term;      /* accumulate negative of the sum */

                /* Update binomial for next k value (if needed). */
                if (k < i)
                {
                    /* binom_next = binom * (i - k) / k   (exact division) */
                    seq_t numer = binom * (seq_t)(i - k);
                    binom = numer / (seq_t)k;
                }

                /* Prepare 3^(k+1) for the next iteration. */
                pow3 *= 3;
            }

            /* Final recurrence: a(i) = -a(i-1) + sum */
            cache[i] = -cache[i - 1] + sum;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
