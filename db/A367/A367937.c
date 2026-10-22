/*
 * A367937  Expansion of e.g.f. exp(4*(exp(x) - 1) + 3*x).
 * https://oeis.org/A367937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367937:
   a(0) = 1
   a(n) = 3*a(n-1) + 4*∑_{k=1..n} binom(n-1, k-1) * a(n-k)          (n ≥ 1)   */

/* This implementation memoises computed terms in a dynamic array.
   It uses only integral arithmetic and the operators defined for
   seq_t.  It compiles under C11 and C++17. */

#include <stdlib.h>

/* Memoisation storage */
static seq_t *cache = NULL;          /* pointer to cached terms          */
static idx_t cache_len = 0;          /* number of terms currently cached */
static size_t cache_cap = 0;         /* allocated capacity of cache      */

/* Ensure that terms a[0] … a[n] are stored in cache.  */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)                     /* already computed           */
        return;

    /* Grow output buffer if necessary.  */
    if (cache_cap == 0)
        cache_cap = 1;
    while (cache_cap <= (size_t)n)
        cache_cap <<= 1;                  /* double until just big enough */

    cache = (seq_t *)realloc(cache,
                             sizeof(seq_t) * cache_cap);
    if (!cache) return;                   /* allocation failure handled
                                            by caller (should not happen) */

    /* Compute new terms, starting from the first missing one.  */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {                     /* a(0) = 1                    */
            cache[0] = 1;
            continue;
        }

        seq_t sum   = 0;                  /* running sum                  */
        seq_t binom = 1;                  /* binom(i-1,0)                 */

        for (idx_t j = 0; j < i; ++j) {
            /* term a(i-1-j) is needed in the recurrence     */
            sum += binom * cache[i - 1 - j];

            /* update binom to binom(i-1, j+1) if more terms remain */
            if (j + 1 < i) {
                binom = (binom * (seq_t)(i - 1 - j)) /
                        (seq_t)(j + 1);
            }
        }

        cache[i] = 3 * cache[i - 1] + 4 * sum;
    }

    cache_len = n + 1;                    /* now terms 0..n are available */
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
