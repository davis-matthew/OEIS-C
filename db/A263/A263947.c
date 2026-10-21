/*
 * A263947  Positive integers n such that (n+57)^3 - n^3 is a square.
 * https://oeis.org/A263947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=551, a(2)=13471, a(3)=67002512, a(4)=1560515752, *
 *            a(5)=7745359676111;
 *            a(n)=a(n-1)+115598*a(n-2)-115598*a(n-3)-a(n-4)+a(n-5) * for n>5.
 *            Indexing starts at 1
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   mul115598, seq_mul_wrapper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul115598.h"
#include "../utils/seq_mul_wrapper.h"

seq_t a(idx_t n)
{
    /* Initial five terms – returned directly. */
    if (n <= 5) {
        static const seq_t init[6] = {
            0ULL,                 /* dummy to keep 1‑based indexing */
            551ULL,               /* a(1) */
            13471ULL,             /* a(2) */
            67002512ULL,          /* a(3) */
            1560515752ULL,        /* a(4) */
            7745359676111ULL      /* a(5) */
        };
        return init[(size_t)n];
    }

    /* Static cache for memoised values. */
    static seq_t *cache = NULL;     /* 1‑based: cache[1] = a(1) */
    static size_t cached = 0;       /* largest n that has been computed */

    /* First allocation – store the five seed values. */
    if (cache == NULL) {
        size_t init_size = 6;       /* 0…5 inclusive */
        cache = (seq_t *)calloc(init_size, sizeof(seq_t));
        if (cache == NULL) return 0;   /* out of memory – unlikely */

        cache[0] = 0;
        cache[1] = 551;
        cache[2] = 13471;
        cache[3] = 67002512;
        cache[4] = 1560515752;
        cache[5] = 7745359676111;
        cached = 5;
    }

    /* Grow the cache if a larger index is requested. */
    if ((size_t)n > cached) {
        size_t new_size = (size_t)n + 1;          /* accommodate 0…n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;                /* out of memory – unlikely */
        cache = tmp;

        /* Compute all missing terms up to n using the recurrence. */
        for (size_t i = cached + 1; i <= n; ++i) {
            /* a(i) = a(i-1) + 115598*a(i-2) - 115598*a(i-3)
               - a(i-4) + a(i-5) */
            seq_t term1 = mul115598(cache[i - 2]);          /* 115598*a(i-2) */
            seq_t term2 = mul115598(cache[i - 3]);          /* 115598*a(i-3) */
            seq_t next   = cache[i - 1] + term1 - term2
                          - cache[i - 4] + cache[i - 5];
            cache[i] = next;
        }
        cached = (size_t)n;
    }

    return cache[(size_t)n];
}
