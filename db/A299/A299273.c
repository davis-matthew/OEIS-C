/*
 * A299273  Partial sums of A299272.
 * https://oeis.org/A299273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-3) - 3*a(n-4) - 3*a(n-6) + 3*a(n-7) +
 *            a(n-9) - a(n-10) a(0) = 1, a(1) = 7, a(2) = 25, a(3) = 62, a(4)
 *            = 125, a(5) = 224, a(6) = 366, a(7) = 555, a(8) = 804, a(9) =
 *            1121
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 1495, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static const seq_t init_vals[10] = {
    (seq_t)1, (seq_t)7, (seq_t)25, (seq_t)62, (seq_t)125,
    (seq_t)224, (seq_t)366, (seq_t)555, (seq_t)804, (seq_t)1121
};

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated size */
    static size_t len = 0;   /* number of terms stored (valid indices 0..len-1) */

    /* initialize cache on first call */
    if (!cache) {
        size_t init_cap = 16;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory: fallback */
        cap = init_cap;
        for (idx_t i = 0; i < 10; ++i) cache[i] = init_vals[i];
        len = 10;
    }

    if (n < len) return cache[n];

    /* Ensure sufficient space for n+1 terms */
    if ((size_t)n + 1 > cap) {
        size_t needed = (size_t)n + 1;
        /* double the capacity until large enough */
        size_t new_cap = cap ? cap : 16;
        while (new_cap < needed) new_cap <<= 1;
        void *p = realloc(cache, new_cap * sizeof(seq_t));
        if (!p) return (seq_t)0;          /* out of memory: fallback */
        cache = (seq_t*)p;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        if (i < 10) {
            cache[i] = init_vals[i];
        } else {
            /* Use signed 64‑bit arithmetic for the linear combination */
            long long v = (long long)cache[i-1]
                         + 3LL * (long long)cache[i-3]
                         - 3LL * (long long)cache[i-4]
                         - 3LL * (long long)cache[i-6]
                         + 3LL * (long long)cache[i-7]
                         + (long long)cache[i-9]
                         - (long long)cache[i-10];
            cache[i] = (seq_t)v;
        }
    }
    len = (size_t)n + 1;
    return cache[n];
}
