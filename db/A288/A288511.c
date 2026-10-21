/*
 * A288511  a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 4*a(n-4) + 4*a(n-5) -
 *          4*a(n-6) + 4*a(n-7) - 4*a(n-8) + 4*a(n-9) - 3*a(n-10) + 2*a(n-11)
 *          - 3*a(n-12) + 2*a(n-13) for n >= 16, with initial values as shown.
 * https://oeis.org/A288511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 4*a(n-4) + 4*a(n-5) -
 *            4*a(n-6) + 4*a(n-7) - 4*a(n-8) + 4*a(n-9) - 3*a(n-10) +
 *            2*a(n-11) - 3*a(n-12) + 2*a(n-13) for n >= 16 initial values:
 *            a(0)=2, a(1)=4, a(2)=5, a(3)=7, a(4)=9, a(5)=12, a(6)=17,
 *            a(7)=23, a(8)=33, a(9)=48, a(10)=70, a(11)=103, a(12)=152, a(...
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 53, u64 n <= 105,
 *            u128 n <= 211, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;
    static size_t cache_cap   = 0;

    /* initial values (indices 0..15) */
    static const seq_t init_vals[16] = {
        2,4,5,7,9,12,17,23,33,48,70,103,152,228,343,515
    };

    /* initialise cache once */
    if (cache_size == 0) {
        cache_cap   = 16;
        cache       = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 16; ++i) cache[i] = init_vals[i];
        cache_size = 16;
    }

    /* grow cache to include requested n */
    while (cache_size <= n) {
        /* grow capacity if needed */
        if (cache_cap <= cache_size) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        idx_t idx = cache_size;          /* compute a[idx] */
        seq_t val = 0;

        /* positive contributions */
        val += cache[idx-1] + cache[idx-1];       /* 2*a(n-1) */
        val += cache[idx-3] + cache[idx-3];       /* 2*a(n-3) */

        seq_t tmp = cache[idx-5] + cache[idx-5];
        tmp += tmp;                               /* 4*a(n-5) */
        val += tmp;

        tmp = cache[idx-7] + cache[idx-7];
        tmp += tmp;                               /* 4*a(n-7) */
        val += tmp;

        tmp = cache[idx-9] + cache[idx-9];
        tmp += tmp;                               /* 4*a(n-9) */
        val += tmp;

        val += cache[idx-11] + cache[idx-11];     /* 2*a(n-11) */
        val += cache[idx-13] + cache[idx-13];     /* 2*a(n-13) */

        /* negative contributions */
        val -= cache[idx-2];                      /* a(n-2) */

        tmp = cache[idx-4] + cache[idx-4];
        tmp += tmp;                               /* 4*a(n-4) */
        val -= tmp;

        tmp = cache[idx-6] + cache[idx-6];
        tmp += tmp;                               /* 4*a(n-6) */
        val -= tmp;

        tmp = cache[idx-8] + cache[idx-8];
        tmp += tmp;                               /* 4*a(n-8) */
        val -= tmp;

        tmp = cache[idx-10] + cache[idx-10] + cache[idx-10]; /* 3*a(n-10) */
        val -= tmp;

        tmp = cache[idx-12] + cache[idx-12] + cache[idx-12]; /* 3*a(n-12) */
        val -= tmp;

        cache[cache_size++] = val;
    }

    return cache[n];
}
