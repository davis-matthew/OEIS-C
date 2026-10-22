/*
 * A366045  Number of circular binary sequences of length n with an odd number
 *          of 0's and no three consecutive 1's.
 * https://oeis.org/A366045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+2*a(n-3)+3*a(n-4)+2*a(n-5)+a(n-6), with a(1)=1,
 *            a(2)=2, a(3)=4, a(4)=4, a(5)=11, a(6)=20
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache: cache[i] holds a(i+1) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0; /* allocated size */
    static idx_t cache_len = 0;  /* highest n computed (>=0) */

    /* initial values a(1)..a(6) */
    static const seq_t init_vals[7] = {
        0ULL,          /* unused */
        1ULL,          /* a(1) */
        2ULL,          /* a(2) */
        4ULL,          /* a(3) */
        4ULL,          /* a(4) */
        11ULL,         /* a(5) */
        20ULL          /* a(6) */
    };

    if (n == 0) return 0;  /* out of range, but harmless */

    /* grow cache if necessary */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute up to n if not already */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 6) {
            val = init_vals[i];
        } else {
            /* recurrence: a(i) = a(i-2)+2*a(i-3)+3*a(i-4)+2*a(i-5)+a(i-6) */
            val = cache[i-3]                               /* a(i-2) */
                + (seq_t)2 * cache[i-4]                    /* 2*a(i-3) */
                + (seq_t)3 * cache[i-5]                    /* 3*a(i-4) */
                + (seq_t)2 * cache[i-6]                    /* 2*a(i-5) */
                + cache[i-7];                              /* a(i-6) */
        }
        cache[i-1] = val;
    }
    cache_len = n;
    return cache[n-1];
}
