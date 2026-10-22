/*
 * A314121  Coordination sequence Gal.6.657.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 3*a(n-3) - 4*a(n-4) + 4*a(n-5) -
 *            4*a(n-6) + 4*a(n-7) - 4*a(n-8) + 3*a(n-9) - 2*a(n-10) +
 *            2*a(n-11) - a(n-12) (n > 12) Initial values: a(0)=1, a(1)=5,
 *            a(2)=11, a(3)=16, a(4)=21, a(5)=26, a(6)=32, a(7)=38, a(8)=42,
 *            a(9)=48, a(10)=54, a(11)=58, a(12)=64
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation (allow random access order) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initial 13 values */
    static const seq_t init[13] = {
        (seq_t)1,  /* a(0) */
        (seq_t)5,  /* a(1) */
        (seq_t)11, /* a(2) */
        (seq_t)16, /* a(3) */
        (seq_t)21, /* a(4) */
        (seq_t)26, /* a(5) */
        (seq_t)32, /* a(6) */
        (seq_t)38, /* a(7) */
        (seq_t)42, /* a(8) */
        (seq_t)48, /* a(9) */
        (seq_t)54, /* a(10) */
        (seq_t)58, /* a(11) */
        (seq_t)64  /* a(12) */
    };

    /* Handle small indices directly */
    if ((idx_t)0 <= n && (size_t)n < 13) {
        return init[(size_t)n];
    }

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_len = 13;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) {
            /* If allocation fails, fall back to on‑the‑fly computation */
            return init[12];  /* should never happen for valid n > 12 */
        }
        for (size_t i = 0; i < cache_len; ++i) {
            cache[i] = init[i];
        }
    }

    /* Extend cache if needed */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) {
            /* If realloc fails, we can't compute the requested term */
            return cache[cache_len - 1];  /* safe fallback */
        }
        cache = tmp;
        for (size_t i = cache_len; i <= n; ++i) {
            unsigned long long pos = 2ULL * (unsigned long long)cache[i-1] +
                                     3ULL * (unsigned long long)cache[i-3] +
                                     4ULL * (unsigned long long)cache[i-5] +
                                     4ULL * (unsigned long long)cache[i-7] +
                                     3ULL * (unsigned long long)cache[i-9] +
                                     2ULL * (unsigned long long)cache[i-11];
            unsigned long long neg = 2ULL * (unsigned long long)cache[i-2] +
                                     4ULL * (unsigned long long)cache[i-4] +
                                     4ULL * (unsigned long long)cache[i-6] +
                                     4ULL * (unsigned long long)cache[i-8] +
                                     2ULL * (unsigned long long)cache[i-10] +
                                     1ULL * (unsigned long long)cache[i-12];
            cache[i] = (seq_t)(pos - neg);
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
