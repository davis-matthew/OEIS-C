/*
 * A295857  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = 0,
 *          a(1) = 0, a(2) = 2, a(3) = 3.
 * https://oeis.org/A295857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4), a(0) = 0, a(1) =
 *            0, a(2) = 2, a(3) = 3
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <new>
#endif

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

/* Ensure the cache can hold up to index n (inclusive). */
static void ensure_capacity(size_t n)
{
    if (cache_cap <= n) {
        size_t new_cap = n + 1;
        if (new_cap < 8) new_cap = 8;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
#else
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
        if (cache) {
            for (size_t i = 0; i < cache_size; ++i) {
                new_cache[i] = cache[i];
            }
#if !defined(__cplusplus)
            free(cache);
#endif
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 4) {
        switch ((int)n) {
            case 0: return (seq_t)0;
            case 1: return (seq_t)0;
            case 2: return (seq_t)2;
            default: return (seq_t)3;   /* n == 3 */
        }
    }

    /* Prepare cache */
    ensure_capacity((size_t)n);

    /* Initialize first four terms if not already done */
    if (cache_size < 4) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache_size = 4;
    }

    /* Compute values up to index n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t val = cache[i - 1]
                    + (seq_t)3 * cache[i - 2]
                    - (seq_t)2 * cache[i - 3]
                    - (seq_t)2 * cache[i - 4];
        cache[i] = val;
    }

    cache_size = (size_t)n + 1;
    return cache[n];
}
