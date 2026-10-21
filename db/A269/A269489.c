/*
 * A269489  Number of length-n 0..3 arrays with no repeated value differing
 *          from the previous repeated value by one.
 * https://oeis.org/A269489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 24*a(n-2) + 9*a(n-3) + 26*a(n-4) + 3*a(n-5)
 *            Recurrence initial values: a(1) = 4 a(2) = 16 a(3) = 64 a(4) =
 *            250 a(5) = 964
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper to multiply seq_t by a small constant without overflowing the
   storage type. Handles both C (numeric seq_t) and C++ (big‑int seq_t)
   builds. */
#ifdef __cplusplus
static inline seq_t mult_small(seq_t a, unsigned int m)
{
    return a * (seq_t)m;
}
#else
#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 tmp128_t;
static inline seq_t mult_small(seq_t a, unsigned int m)
{
    return (seq_t)((tmp128_t)a * m);
}
#else
static inline seq_t mult_small(seq_t a, unsigned int m)
{
    return (seq_t)((unsigned long long)a * m);
}
#endif
#endif

static seq_t *cache = NULL;      /* array of computed terms, 1‑based index */
static idx_t cache_alloc = 0;    /* number of allocated slots (size of array) */
static idx_t cache_filled = 0;   /* highest index that has already been computed */

/* Ensure the cache can hold at least up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Allocate enough space to hold indices 0..5 (six slots). */
        cache_alloc = 6;
        cache = (seq_t *)malloc(cache_alloc * sizeof(seq_t));
        if (cache == NULL)
            abort();
        cache[0] = (seq_t)0;   /* unused, kept for 1‑based access */
        cache[1] = (seq_t)4;   /* a(1) */
        cache[2] = (seq_t)16;  /* a(2) */
        cache[3] = (seq_t)64;  /* a(3) */
        cache[4] = (seq_t)250; /* a(4) */
        cache[5] = (seq_t)964; /* a(5) */
        cache_filled = 5;
    }
    if (n + 1 > cache_alloc) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            abort();
        cache = tmp;
        cache_alloc = new_size;
    }
}

/* Main sequence function, computes a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    ensure_cache(n);

    if (n > cache_filled) {
        for (idx_t i = cache_filled + 1; i <= n; ++i) {
            seq_t sum = mult_small(cache[i-1], 9) +
                        mult_small(cache[i-3], 9) +
                        mult_small(cache[i-4], 26) +
                        mult_small(cache[i-5], 3);
            seq_t sub = mult_small(cache[i-2], 24);
            cache[i] = sum - sub;
        }
        cache_filled = n;
    }

    return cache[n];
}
