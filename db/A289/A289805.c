/*
 * A289805  p-INVERT of A103889, where p(S) = 1 - S - S^2.
 * https://oeis.org/A289805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) - 8*a(n-3) + 8*a(n-4) - 2*a(n-5) +
 *            a(n-6) Initial values: a(0)=2, a(1)=9, a(2)=36, a(3)=153,
 *            a(4)=624, a(5)=2584 This implementation memoizes previously
 *            computed terms to handle calls in arbitrary order. It uses only
 *            integer operations and dynamic allocation
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* pointer to cached sequence terms */
static size_t cache_len = 0;    /* number of terms currently cached (last index + 1) */
static size_t cache_cap = 0;    /* allocated capacity of cache */

/* Ensure that the cache has at least |need| elements allocated. */
static void ensure_cache(size_t need) {
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < need) new_cap <<= 1;           /* grow to at least need */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Initialise cache with the first six terms if it has not been done yet. */
static void init_base_terms(void) {
    if (cache_len < 6) {
        ensure_cache(6);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)36;
        cache[3] = (seq_t)153;
        cache[4] = (seq_t)624;
        cache[5] = (seq_t)2584;
        cache_len = 6;
    }
}

seq_t a(idx_t n) {
    /* Return the first six terms directly, avoiding any allocation. */
    if (n < 6) {
        switch ((idx_t)n) {
            case 0: return (seq_t)2;
            case 1: return (seq_t)9;
            case 2: return (seq_t)36;
            case 3: return (seq_t)153;
            case 4: return (seq_t)624;
            case 5: return (seq_t)2584;
            default: return (seq_t)0;   /* unreachable, but keeps compiler happy */
        }
    }

    /* Ensure we have the base terms cached */
    init_base_terms();

    size_t u_n = (size_t)n;   /* cast to size_t for array indexing (n up to 10,000) */

    /* Expand cache if necessary */
    if (u_n + 1 > cache_cap)
        ensure_cache(u_n + 1);

    /* Compute any missing terms up to n */
    if (u_n >= cache_len) {
        for (size_t i = cache_len; i <= u_n; ++i) {
            /* Compute a(i) using the recurrence:
               a(i) = (4*a(i-1) + 2*a(i-2) + 8*a(i-4) + a(i-6))
                      - (8*a(i-3) + 2*a(i-5))
            */
            seq_t pos = (4 * cache[i-1] + 2 * cache[i-2]
                        + 8 * cache[i-4] + cache[i-6]);
            seq_t neg = 8 * cache[i-3] + 2 * cache[i-5];
            cache[i] = pos - neg;
        }
        cache_len = u_n + 1;
    }

    return cache[u_n];
}
