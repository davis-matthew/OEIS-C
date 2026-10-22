/*
 * A371888  G.f. A(x) satisfies A(x) = 1 - x/A(x) * (1 - A(x) - A(x)^2).
 * https://oeis.org/A371888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 30, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: n*a(n)+3*(-n+1)*a(n-1)+(7*n-18)*a(n-2)+5*(-n+3)*a(n-3)=0
   (valid for n ≥ 4) */
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
static seq_t *cache = nullptr;
static std::size_t cache_len = 0;
static std::size_t cache_cap = 0;
#else
static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static idx_t  cache_cap = 0;
#endif

static void ensure_cache(idx_t n) {
    /* Allocate or grow cache to at least n+1 elements */
    idx_t needed = n + 1;
    if (cache_cap < needed) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_cap];
#else
        new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
        if (!new_cache) exit(1);
        /* Copy existing data */
        for (idx_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        /* Free old storage */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Ensure initial values are present */
    if (cache_len < 4) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache_len = 4;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term =
            (seq_t)3 * (seq_t)(i - 1) * cache[i - 1]
            - (seq_t)(7 * i - 18) * cache[i - 2]
            + (seq_t)5 * (seq_t)(i - 3) * cache[i - 3];
        cache[i] = term / (seq_t)i;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    ensure_cache(n);
    return cache[n];
}
