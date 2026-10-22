/*
 * A352182  Twice A200994.
 * https://oeis.org/A352182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 99*a(n-1) - 99*a(n-2) + a(n-3) for n > 2 a(0) = 0, a(1) =
 *            30, a(2) = 2970
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, realloc, free (C) / operator new/delete (C++) */

static seq_t *cache = NULL;          /* raw storage of the sequence */
static idx_t computed = 0;          /* number of computed terms (indices 0 .. computed-1) */
static idx_t cache_cap = 0;         /* allocated capacity of 'cache' */

/* Ensure that the values up to index 'n' have been computed and stored. */
static void
ensure_cache(idx_t n)
{
    /* If everything already exists, do nothing. */
    if (n < computed)
        return;

    /* Grow the array if necessary. */
    if (!cache) {
        /* First allocation: at least n+1 slots, but round up for growth. */
        idx_t new_cap = (n + 1 > 16) ? n + 1 : 16;
        if (new_cap < n + 1)
            new_cap = n + 1;
#if __cplusplus
        cache = (seq_t*)::operator new(new_cap * sizeof(seq_t));
#else
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
        cache_cap = new_cap;
    } else if (n + 1 > cache_cap) {
        /* Reallocate with exponential growth. */
        idx_t new_cap = cache_cap * 2;
        if (new_cap < 16) new_cap = 16;
        while (new_cap <= n)
            new_cap *= 2;
#if __cplusplus
        seq_t *new_cache = (seq_t*)::operator new(new_cap * sizeof(seq_t));
        /* Copy existing values. */
        for (idx_t i = 0; i < computed; ++i)
            new_cache[i] = cache[i];
        ::operator delete[](cache);
        cache = new_cache;
#else
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
#endif
        cache_cap = new_cap;
    }

    /* Compute any missing terms. */
    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0)
            cache[i] = 0;
        else if (i == 1)
            cache[i] = 30;
        else if (i == 2)
            cache[i] = 2970;
        else
            cache[i] = 99 * cache[i-1] - 99 * cache[i-2] + cache[i-3];
    }
    computed = n + 1;
}

/* External linkage function requested by the build. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
