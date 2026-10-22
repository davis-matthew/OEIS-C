/*
 * A305862  a(n) = 384*4^n - 576*3^n + 220*2^n - 14.
 * https://oeis.org/A305862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A305862
   Formula: a(n) = 384*4^n - 576*3^n + 220*2^n - 14
   Recurrence: a(n) = 10*a(n-1) - 35*a(n-2) + 50*a(n-3) - 24*a(n-4)
   Initial values: a(0)=14, a(1)=234, a(2)=1826, a(3)=10770
   The function below memoises computed terms in a static array so that
   arbitrary order of calls is supported.  Only standard C11 operations
   are used; the big‑int utilities provide accurate results for the
   arbitrary‑precision build while 64‑bit builds are correct up to the
   largest term that fits. */
#include <stdlib.h>

static seq_t *cache = NULL;          // dynamically allocated cache
static size_t cache_len = 0;         // number of computed values
static size_t cache_cap = 0;         // allocated capacity

seq_t a(idx_t n) {
    /* Ensure the cache has enough space.  Use realloc for growth. */
    if (cache == NULL) {
        /* Allocate at least four slots for the initial terms. */
        size_t init = (n >= 3) ? (n + 1) : 4;
        cache_cap = init;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();           // cannot allocate

        /* Set initial values */
        cache[0] = 14;
        cache[1] = 234;
        cache[2] = 1826;
        cache[3] = 10770;
        cache_len = 4;
    } else if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n, if any. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 4) {
            /* Initial terms are already set. */
            continue;
        }
        /* Use recurrence a(i) = 10*a(i-1) - 35*a(i-2) + 50*a(i-3) - 24*a(i-4) */
        seq_t t1 = cache[i-1];
        seq_t t2 = cache[i-2];
        seq_t t3 = cache[i-3];
        seq_t t4 = cache[i-4];
        seq_t val = 10 * t1 - 35 * t2 + 50 * t3 - 24 * t4;
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
