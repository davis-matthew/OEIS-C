/*
 * A373578  Expansion of e.g.f. exp(x * (1 + x^2)^2).
 * https://oeis.org/A373578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*(n-1)*(n-2)*a(n-3) +
 *            5*(n-1)*(n-2)*(n-3)*(n-4)*a(n-5) a(0) = 1, a(1) = 1, a(2) = 1,
 *            a(3) = 13, a(4) = 49
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity in entries */
    static size_t cache_size = 0;  /* number of valid entries */

    /* Initialise cache with the first five terms */
    if (cache == NULL) {
        cache_cap = 5;
        cache_size = 5;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)13;
        cache[4] = (seq_t)49;
    }

    /* Ensure capacity for up to n */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n)
            new_cap <<= 1;
        if (new_cap == 0) new_cap = 4; /* guard against zero */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp != NULL) {
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    /* If value already known, return it immediately */
    if (n < cache_size)
        return cache[(size_t)n];

    /* Compute terms up to n */
    for (idx_t k = (idx_t)cache_size; k <= n; ++k) {
        if (k < 5) {
            /* initial values already stored */
            continue;
        }

        seq_t term1 = cache[(size_t)(k - 1)];

        /* compute term2 = 6 * (k-1) * (k-2) * cache[k-3] */
        unsigned long long coeff2 =
            (unsigned long long)6 *
            (unsigned long long)(k - 1) *
            (unsigned long long)(k - 2);
        seq_t term2 = (seq_t)coeff2 * cache[(size_t)(k - 3)];

        /* compute term3 = 5 * (k-1)*(k-2)*(k-3)*(k-4) * cache[k-5] */
        unsigned long long coeff3 =
            (unsigned long long)5 *
            (unsigned long long)(k - 1) *
            (unsigned long long)(k - 2) *
            (unsigned long long)(k - 3) *
            (unsigned long long)(k - 4);
        seq_t term3 = (seq_t)coeff3 * cache[(size_t)(k - 5)];

        cache[(size_t)k] = term1 + term2 + term3;
    }

    cache_size = (size_t)(n + 1);
    return cache[(size_t)n];
}
