/*
 * A285065  Alternating row sums of Sheffer triangle S2[4,1] = A285061.
 * https://oeis.org/A285065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 11, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A285065
   Formula (recurrence):
   a(0) = 1
   a(n) = a(n-1) - Σ_{k=1..n}  binomial(n-1, k-1) * 4^k * a(n-k),  n ≥ 1
   This implementation memoises the values up to the largest n requested
   and recomputes only those that are missing.
   The cache entries are initialised to zero after each reallocation, so
   the code works correctly for successive calls in any order. */

#include <stdlib.h>

/* cache for memoised values */
static seq_t *cache      = NULL;   /* cache array */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_len  = 0;      /* number of values that are known */

/* Ensure the cache has room for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);   /* out of memory */

    /* initialise the newly allocated slots to zero */
    for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;

    cache     = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* make sure we can store the requested index */
    ensure_capacity(n);

    /* Initialise the first element when the cache is empty */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }

    /* Compute missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t sum = 0;

        /* Compute the summation part of the recurrence */
        seq_t binom = 1;     /* C(i-1,0) for k=1 */
        seq_t pow4  = 4;     /* 4^1 for k=1 */

        for (size_t k = 1; k <= i; ++k) {
            /* add term binom * 4^k * a(i-k) */
            sum += binom * pow4 * cache[i - k];

            if (k < i) {
                /* Update binom to C(i-1, k)  */
                binom = binom * (seq_t)(i - k) / (seq_t)(k);
                /* Next power of 4 */
                pow4 *= 4;
            }
        }

        /* recurrence: a(i) = a(i-1) - sum */
        cache[i] = cache[i - 1] - sum;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
