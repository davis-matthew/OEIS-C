/*
 * A360048  a(n) = Sum_{k=0..floor(n/2)} (-1)^k * binomial(n+1,2*k+1) *
 *          Catalan(k).
 * https://oeis.org/A360048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} (-1)^k * binomial(n+1, 2k+1) * Catalan(k)
 *            Recurrence (used for fast O(n^2) computation): a(0)=1, a(1)=2,
 *            a(n) = (n+1) - Σ_{k=0}^{n-2} a(k) * a(n-2-k) for n ≥ 2
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 32, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of previously computed a(n) */
static idx_t  cache_len = 0;  /* number of elements stored in cache */
static size_t cache_cap = 0;  /* allocated capacity of cache         */

/* Ensure cache has at least (n+1) elements, initializing missing entries. */
static void
ensure_cache(idx_t n)
{
    /* Allocate or enlarge cache if necessary */
    if (cache == NULL) {
        cache_cap = (size_t)((n + 1) > 1024 ? (n + 1) : 1024);
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return;  /* leave to caller to handle OOM */

        /* Base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_len = (n < 1 ? 1 : 2);
    } else if (cache_cap <= n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return;  /* OOM: caller must handle */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)2;
        } else {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= i - 2; ++k) {
                sum += cache[k] * cache[i - k - 2];
            }
            cache[i] = ((seq_t)i + (seq_t)1) - sum;
        }
    }
    cache_len = n + 1;
}

seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
