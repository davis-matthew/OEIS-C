/*
 * A355411  Expansion of e.g.f. 1/(3 - exp(2*x) - exp(3*x)).
 * https://oeis.org/A355411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A355411
   Formula: a(0) = 1
            a(n) = Σ_{k=1..n} (3^k + 2^k) * binomial(n,k) * a(n-k)   (n ≥ 1) */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

#ifdef __cplusplus
#  include <new>
#endif

/* Ensure the cache can hold indices up to n */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    if (new_cap < need) new_cap = need;

    void *raw = NULL;
#ifdef __cplusplus
    raw = ::operator new(new_cap * sizeof(seq_t));
#else
    raw = malloc(new_cap * sizeof(seq_t));
#endif
    if (!raw) return;                    /* allocation failed – give up */

    seq_t *new_cache =
#ifdef __cplusplus
        reinterpret_cast<seq_t *>(raw);
#else
        (seq_t *)raw;
#endif
    /* copy old data */
    for (size_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];
    /* zero initialise the new part */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = (seq_t)0;

    /* free old block */
    if (cache) {
#ifdef __cplusplus
        ::operator delete(reinterpret_cast<void *>(cache));
#else
        free(cache);
#endif
    }

    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);

    /* initialise a(0) once */
    if (cache[0] == (seq_t)0) cache[0] = (seq_t)1;

    for (size_t i = (size_t)n; i < cache_cap; ++i) {
        if (i == 0) continue;            /* already set */

        seq_t sum = (seq_t)0;

        seq_t binom = (seq_t)1;          /* binomial(i,0) */
        seq_t pow2 = (seq_t)2;           /* 2^1   */
        seq_t pow3 = (seq_t)3;           /* 3^1   */

        for (idx_t k = 1; k <= (idx_t)i; ++k) {
            /* binomial(i,k) = binom * (i-k+1) / k */
            binom = binom * (seq_t)(i - k + 1);
            binom = binom / (seq_t)k;

            seq_t term = pow2 + pow3;
            term = term * binom;
            term = term * cache[i - k];
            sum += term;

            /* update powers for next k */
            pow2 = pow2 * (seq_t)2;
            pow3 = pow3 * (seq_t)3;
        }

        cache[i] = sum;
    }

    return cache[n];
}
