/*
 * A361572  Expansion of e.g.f. exp( (x / (1-x))^3 ).
 * https://oeis.org/A361572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(n-1)*a(n-1) - 6*(n-2)*(n-1)*a(n-2) +
 *            (n-2)*(n-1)*(4*n-9)*a(n-3) - (n-4)*(n-3)*(n-2)*(n-1)*a(n-4) For
 *            n ≥ 4 with initial values a(0)=1, a(1)=0, a(2)=0, a(3)=6
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = 0;

static void ensure_cache_size(idx_t need)
{
#if defined(__cplusplus)
    if ((idx_t)cache_cap > need) return;
    size_t new_cap = cache_cap? cache_cap << 1 : 8;
    while (new_cap <= (size_t)need) new_cap <<= 1;
    seq_t *new_cache = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        delete[] cache;
    }
    cache = new_cache;
    cache_cap = new_cap;
#else
    if ((idx_t)cache_cap > need) return;
    size_t new_cap = cache_cap? cache_cap << 1 : 8;
    while (new_cap <= (size_t)need) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);              /* fatal on allocation failure */
    if (!cache) {
        /* zero‑initialise any newly allocated elements */
        for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
    }
    cache = new_cache;
    cache_cap = new_cap;
#endif
}

seq_t a(idx_t n)
{
    /* initialise base values only once */
    if (!cache) {
        ensure_cache_size(3);
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 6;
        last_computed = 3;
    }

    if (n <= last_computed) return cache[n];

    ensure_cache_size(n);
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* recurrence valid for i ≥ 4 */
        seq_t term1 = 4 * (seq_t)(i - 1) * cache[i - 1];
        seq_t term2 = 6 * (seq_t)(i - 2) * (i - 1) * cache[i - 2];
        seq_t term3 = (seq_t)(i - 2) * (i - 1)
                      * ((seq_t)(4 * i) - 9) * cache[i - 3];
        seq_t term4 = (seq_t)(i - 4) * (i - 3) * (i - 2) * (i - 1)
                      * cache[i - 4];
        cache[i] = term1 - term2 + term3 - term4;
    }
    last_computed = n;
    return cache[n];
}
