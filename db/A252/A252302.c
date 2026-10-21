/*
 * A252302  Number of (n+2)X(5+2) 0..3 arrays with every 3X3 subblock row and
 *          diagonal sum equal to 0 1 3 6 or 7 and every 3X3 column and
 *          antidiagonal sum not equal to 0 1 3 6 or 7.
 * https://oeis.org/A252302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-3) - 70*a(n-6) + 120*a(n-9) - 64*a(n-12) for n >
 *            21 Initial values: a(1) = 4349 , a(2) = 3171 , a(3) = 3902 ,
 *            a(4) = 5230 , a(5) = 9368 , a(6) = 14515 , a(7) = 20910 , a(8) =
 *            42082 , a(9) = 66739 , a(10) = 98782 , a(11) = 217314 , a(12) =
 *            348739 , a(13) = 531390 , a(14) = 1284226 ,...
 * Range:     u8 none, u16 n <= 8, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_vals[22] = {
    0,                         /* dummy 0‑th index */
    4349,                      /* a(1) */
    3171,                      /* a(2) */
    3902,                      /* a(3) */
    5230,                      /* a(4) */
    9368,                      /* a(5) */
    14515,                     /* a(6) */
    20910,                     /* a(7) */
    42082,                     /* a(8) */
    66739,                     /* a(9) */
    98782,                     /* a(10) */
    217314,                    /* a(11) */
    348739,                    /* a(12) */
    531390,                    /* a(13) */
    1284226,                   /* a(14) */
    2070979,                   /* a(15) */
    3235390,                   /* a(16) */
    8478402,                   /* a(17) */
    13717315,                  /* a(18) */
    21847614,                  /* a(19) */
    60676930,                  /* a(20) */
    98374723                   /* a(21) */
};

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size */
    static size_t cache_len = 0;   /* largest index computed */

    if (n <= 21) {
        return init_vals[(idx_t)n];
    }

    /* init cache */
    if (!cache) {
        cache_cap = 22;                /* indices 0..21 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed */
        cache[0] = 0;
        for (idx_t i = 1; i <= 21; ++i) {
            cache[i] = init_vals[i];
        }
        cache_len = 21;
    }

    /* extend cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* fill missing entries */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        seq_t t1 = cache[i - 3] * (seq_t)15;
        seq_t t2 = cache[i - 9] * (seq_t)120;
        seq_t sum1 = t1 + t2;

        seq_t t3 = cache[i - 6] * (seq_t)70;
        seq_t t4 = cache[i - 12] * (seq_t)64;
        seq_t sum2 = t3 + t4;

        cache[i] = sum1 - sum2;
    }
    cache_len = (size_t)n;
    return cache[n];
}
