/*
 * A298456  Number of nX3 0..1 arrays with every element equal to 2, 3, 4, 5
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the 13‑term recurrence a(n) =
 *            4a(n-1)+10a(n-2)-19a(n-3)-42a(n-4)+17a(n-5)+27a(n-6)
 *            -39a(n-7)+45a(n-8)+75a(n-9)-35a(n-10)-6a(n-11)+6a(n-12)-8a(n-13)
 *            with initial values a(1)=0, a(2)=3, a(3)=8, a(4)=30, a(5)=153,
 *            a(6)=710, a(7)=3490, a(8)=17087, a(9)=84008, a(10)=413964,
 *            a(11)=20...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* behaviour below offset is undefined */

    /* Static cache persistent across calls */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size (not counting index 0) */
    static size_t cache_len = 0;   /* largest n whose value is stored */

    /* Constant initial values (index 0 unused) */
    static const seq_t init_vals[16] = {
        0,          /* 0th unused element */
        0,          /* a(1) */
        3,          /* a(2) */
        8,          /* a(3) */
        30,         /* a(4) */
        153,        /* a(5) */
        710,        /* a(6) */
        3490,       /* a(7) */
        17087,      /* a(8) */
        84008,      /* a(9) */
        413964,     /* a(10) */
        2040233,    /* a(11) */
        10061180,   /* a(12) */
        49619258,   /* a(13) */
        244733629,  /* a(14) */
        1207118898  /* a(15) */
    };

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;                     /* capacity for indices 0..15 */
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
#endif
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = 0;
        for (size_t i = 1; i <= 15; ++i)
            cache[i] = init_vals[i];
        cache_len = 15;
    }

    /* Grow the cache if necessary (C++ mode) */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
#ifdef __cplusplus
        seq_t *newc = new seq_t[new_cap];
        for (size_t i = 0; i < cache_cap; ++i)
            newc[i] = cache[i];
        delete[] cache;
        cache = newc;
#else
        seq_t *newc = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newc) exit(1);                /* allocation failure */
        cache = newc;
#endif
        /* initialise new entries to zero */
        for (size_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if ((size_t)n > cache_len) {
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
            cache[i] = 4  * cache[i-1]
                     + 10 * cache[i-2]
                     - 19 * cache[i-3]
                     - 42 * cache[i-4]
                     + 17 * cache[i-5]
                     + 27 * cache[i-6]
                     - 39 * cache[i-7]
                     + 45 * cache[i-8]
                     + 75 * cache[i-9]
                     - 35 * cache[i-10]
                     - 6  * cache[i-11]
                     + 6  * cache[i-12]
                     - 8  * cache[i-13];
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)n];
}
