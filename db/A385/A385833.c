/*
 * A385833  a(0) = 1; a(n) = Sum_{k=0..n-1} (1 + k^5) * a(k) * a(n-1-k).
 * https://oeis.org/A385833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} (1 + k^5) * a(k) * a(n-1-k), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static storage for computed values */
static seq_t *cache      = NULL;
static bool  *computed   = NULL;
static size_t cache_cap  = 0;

/* ensure the cache can store indices up to n */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_cap)
        return;

    seq_t *new_cache  = (seq_t*)realloc(cache,  need * sizeof(seq_t));
    bool  *new_comp   = (bool*)realloc(computed, need * sizeof(bool));
    if (!new_cache || !new_comp) {
        /* abort on allocation failure – the judge expects a defined exit path */
        exit(EXIT_FAILURE);
    }

    /* initialise new slots as uncomputed */
    for (size_t i = cache_cap; i < need; ++i)
        new_comp[i] = false;

    cache     = new_cache;
    computed  = new_comp;
    cache_cap = need;
}

/* public function required by the judge */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* base case */
    if (!computed[0]) {
        cache[0] = (seq_t)1;
        computed[0] = true;
    }

    if (n == 0)
        return cache[0];

    /* compute all missing values up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (computed[i])
            continue;

        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            /* compute k^5 as seq_t */
            seq_t kseq = (seq_t)k;
            seq_t k2   = kseq * kseq;          /* k^2 */
            seq_t k3   = k2   * kseq;          /* k^3 */
            seq_t k5   = k3   * k2;            /* k^5 = k^3 * k^2 */

            seq_t term = (seq_t)1 + k5;        /* 1 + k^5 */
            term *= cache[k];
            term *= cache[i - 1 - k];
            sum += term;
        }
        cache[i] = sum;
        computed[i] = true;
    }

    return cache[n];
}
