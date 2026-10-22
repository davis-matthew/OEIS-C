/*
 * A375288  Expansion of 1/((1 - x + x^3)^2 + 4*x^4).
 * https://oeis.org/A375288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 16, i32 n <= 34, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A375288: Expansion of 1/((1 - x + x^3)^2 + 4*x^4)
   Recurrence: a(n) = 2*a(n-1) - a(n-2) - 2*a(n-3) - 2*a(n-4) - a(n-6)
   Initial values (n=0..5): 1, 2, 3, 2, -5, -22 */
#include <stdlib.h>
#include <stdint.h>

/* Static cache for memoization */
static seq_t *cache = NULL;
static unsigned char *computed = NULL;   /* 0 = not computed, 1 = computed */
static size_t cache_size = 0;

/* Ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n) {
    size_t required = (size_t)n + 1;
    if (required > cache_size) {
        seq_t *new_cache = (seq_t *)malloc(required * sizeof(seq_t));
        unsigned char *new_comp = (unsigned char *)calloc(required, sizeof(unsigned char));
        if (cache) free(cache);
        if (computed) free(computed);
        cache = new_cache;
        computed = new_comp;
        cache_size = required;
    }
}

/* External linkage function requested by the build system */
seq_t a(idx_t n) {
    static const seq_t base[6] = {1, 2, 3, 2, -5, -22};

    /* Handle n < 6 directly */
    if (n < 6) return base[n];

    ensure_cache(n);

    /* Compute all values up to n if not yet computed */
    for (idx_t i = 0; i <= n; ++i) {
        if (i < 6) {
            cache[i] = base[i];
            computed[i] = 1;
            continue;
        }
        if (computed[i]) continue;
        seq_t val = 2 * cache[i - 1]          /* 2*a(n-1) */
                 - cache[i - 2]                /* -a(n-2) */
                 - 2 * cache[i - 3]            /* -2*a(n-3) */
                 - 2 * cache[i - 4]            /* -2*a(n-4) */
                 -       cache[i - 6];         /* -a(n-6) */
        cache[i] = val;
        computed[i] = 1;
    }
    return cache[n];
}
