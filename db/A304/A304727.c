/*
 * A304727  a(0) = 0, a(1) = 1, a(n) = n! * a(n-1) + a(n-2).
 * https://oeis.org/A304727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, a(n) = n! * a(n-1) + a(n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#define NEW_CACHE(sz) new seq_t[(sz)]
#define FREE_CACHE(ptr) delete[] (ptr)
#else
#define NEW_CACHE(sz) ((seq_t*)malloc((sz)*sizeof(seq_t)))
#define FREE_CACHE(ptr) free(ptr)
#endif

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static seq_t last_fact = 0; /* factorial of fact_index */
    static idx_t fact_index = 0;

    /* base values handled directly */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* initialise cache if needed */
    if (!cache) {
        cache_size = 2u;
        cache = NEW_CACHE(cache_size);
        cache[0] = 0;
        cache[1] = 1;
        last_fact = 1;      /* 1! */
        fact_index = 1;
    }

    /* extend cache to include n */
    if (cache_size <= (size_t)n) {
        size_t old_size = cache_size;
        size_t new_size = (size_t)n + 1u;

        seq_t *new_cache = NEW_CACHE(new_size);
        for (size_t i = 0; i < old_size; ++i) {
            new_cache[i] = cache[i];
        }
        FREE_CACHE(cache);
        cache = new_cache;
        cache_size = new_size;

        for (idx_t i = (idx_t)old_size; i <= n; ++i) {
            /* compute factorial(i) */
            last_fact = last_fact * (seq_t)i;  /* last_fact now = i! */
            fact_index = i;

            /* recurrence: a(i) = i! * a(i-1) + a(i-2) */
            cache[i] = last_fact * cache[i - 1] + cache[i - 2];
        }
    }

    return cache[n];
}
