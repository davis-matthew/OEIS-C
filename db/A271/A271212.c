/*
 * A271212  a(n) = (2n-1)*a(n-1) + 2*(n-2)*a(n-2).
 * https://oeis.org/A271212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n-1)*a(n-1) + 2*(n-2)*a(n-2) (a(0)=1, a(1)=2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed_max = 0;
    static bool initialized = false;

    if (!initialized) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed – return 0 */
        cache[0] = (seq_t)1;          /* a(0) */
        cache[1] = (seq_t)2;          /* a(1) */
        computed_max = 1;
        initialized = true;
    }

    /* If n is already computed, return it immediately */
    if (n <= computed_max)
        return cache[n];

    /* Ensure the cache can hold up to n */
    if ((size_t)(n + 1) > cache_cap) {
        cache_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* reallocation failed */
        cache = tmp;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t term1 = ((seq_t)(2 * i - 1)) * cache[i - 1];
        seq_t term2 = ((seq_t)(2 * (i - 2))) * cache[i - 2];
        cache[i] = term1 + term2;
    }
    computed_max = n;
    return cache[n];
}
