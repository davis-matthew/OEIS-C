/*
 * A354738  a(0) = a(1) = 1; a(n) = (n-1) * Sum_{k=0..n-2} a(k) * a(n-k-2).
 * https://oeis.org/A354738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A354738:  a(0) = a(1) = 1;
   a(n) = (n-1) * Σ_{k=0}^{n-2} a(k) * a(n-k-2)       (n ≥ 2) */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* cached a(k) values               */
static size_t cache_len = 0;         /* number of values actually stored */
static size_t cache_cap = 0;         /* capacity of the allocated array  */

/* Ensure the cache can address index n (= array size n+1). */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;                       /* already large enough */

    /* grow to at least n+1 */
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    if (new_cap < (size_t)n + 1)
        new_cap = (size_t)n + 1;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return;           /* allocation failure – undefined */

    /* copy old values */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* free old array and update variables */
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return a(n) for arbitrary-precision or fixed-size seq_t. */
seq_t a(idx_t n)
{
    /* initialise base values once */
    if (cache_len < 2) {
        ensure_cache_capacity(1);      /* array of size 2 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* grow array if necessary */
    if ((size_t)n + 1 > cache_cap)
        ensure_cache_capacity(n);

    /* compute any missing terms */
    if (cache_len <= (size_t)n) {
        for (size_t idx = cache_len; idx <= (size_t)n; ++idx) {
            seq_t sum = (seq_t)0;
            for (size_t k = 0; k <= idx - 2; ++k)
                sum += cache[k] * cache[idx - k - 2];
            cache[idx] = ((seq_t)(idx - 1)) * sum;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
