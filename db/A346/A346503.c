/*
 * A346503  G.f. A(x) satisfies A(x) = 1 + x^3 * A(x)^2 / (1 - x).
 * https://oeis.org/A346503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(1) = a(2) = 0; a(n) = a(n-1) + Sum_{k=0..n-3} a(k) *
 *            a(n-k-3) for n ≥ 3 (OEIS A346503)
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 39, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* dynamic array holding a(0 .. max_needed) */
static seq_t *cache = NULL;
/* current capacity of cache (number of stored entries) */
static size_t capacity = 0;
/* greatest index that has been computed so far, or (idx_t)-1 if none */
static idx_t computed = (idx_t)-1;

/* ensure cache can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        capacity = 16;
        if (n + 1 > capacity)
            capacity = n + 1;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache)
            abort();
        for (size_t i = 0; i < capacity; ++i)
            cache[i] = (seq_t)0;
    } else if (n + 1 > capacity) {
        size_t new_cap = capacity;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        /* initialize new entries */
        for (size_t i = capacity; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        capacity = new_cap;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* make cache large enough */
    ensure_cache(n);

    /* initialise first values if this is the first call */
    if (computed == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        computed = 2;
    }

    /* compute any missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i)
    {
        if (i < 3) {                /* safety, although i≥3 here */
            cache[i] = (seq_t)0;
            continue;
        }
        seq_t sum = cache[i - 1];
        for (idx_t k = 0; k <= i - 3; ++k)
        {
            sum += cache[k] * cache[i - k - 3];
        }
        cache[i] = sum;
    }

    computed = n;
    return cache[n];
}
