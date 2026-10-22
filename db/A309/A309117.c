/*
 * A309117  Number of perfect matchings on a triangular lattice of width 4 and
 *          length n.
 * https://oeis.org/A309117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies a(0)=1, a(1)=1, a(2)=5, a(3)=15, a(4)=56, *
 *            a(5)=203, a(6)=749, a(7)=2777; * for n>=8: a(n) = 2*a(n-1) +
 *            9*a(n-2) - 4*a(n-3) - 21*a(n-4) * - 4*a(n-5) + 9*a(n-6) +
 *            2*a(n-7) - a(n-8)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Internal cache for memoised values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Initial eight terms */
static const seq_t init_terms[8] = {
    (seq_t)1,
    (seq_t)1,
    (seq_t)5,
    (seq_t)15,
    (seq_t)56,
    (seq_t)203,
    (seq_t)749,
    (seq_t)2777
};

/* Ensure the cache can hold at least n+1 terms */
static void ensure_cache_capacity(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < required)
        new_cap = required;
    if (cache == NULL)
    {
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (cache == NULL)
            abort(); /* allocation failed */
    }
    else
    {
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            abort();
        cache = tmp;
    }
    cache_cap = new_cap;
}

/* Compute a(n) with memoisation */
seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n < 8)
        return init_terms[(size_t)n];

    ensure_cache_capacity(n);

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        if (i < 8)
        {
            cache[i] = init_terms[i];
            continue;
        }
        /* Recurrence:
         * a[i] = 2*a[i-1] + 9*a[i-2] - 4*a[i-3] - 21*a[i-4]
         *        - 4*a[i-5] + 9*a[i-6] + 2*a[i-7] - a[i-8];
         */
        seq_t val = cache[i-1];
        val = val * (seq_t)2;
        val += cache[i-2] * (seq_t)9;
        val -= cache[i-3] * (seq_t)4;
        val -= cache[i-4] * (seq_t)21;
        val -= cache[i-5] * (seq_t)4;
        val += cache[i-6] * (seq_t)9;
        val += cache[i-7] * (seq_t)2;
        val -= cache[i-8];
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
