/*
 * A387171  Number of 4 element sets of distinct integer sided rectangles that
 *          fill an n X n square.
 * https://oeis.org/A387171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -a(n-1) + a(n-2) + 3*a(n-3) + 3*a(n-4) - a(n-5) -
 *            4*a(n-6) - 4*a(n-7) - a(n-8) + 3*a(n-9) + 3*a(n-10) + a(n-11) -
 *            a(n-12) - a(n-13), for n > 13, with initial values a(1)=0,
 *            a(2)=0, a(3)=0, a(4)=3, a(5)=15, a(6)=35, a(7)=75, a(8)=119,
 *            a(9)=210, a(10)=289, a(11)=441, a(12)=574, a(13)=804. (A...
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset. */
    if (n < 1) return 0;

    /* Static cache: cache[i] holds a(i) for i <= cache_len. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* maximum n that has been computed */
    static size_t cache_cap = 0;   /* allocated capacity of cache */

    /* Initial values up to n = 13 are known. */
    const seq_t init_vals[14] = {
        0,   /* a(0) – unused */
        0,   /* a(1) */
        0,   /* a(2) */
        0,   /* a(3) */
        3,   /* a(4) */
        15,  /* a(5) */
        35,  /* a(6) */
        75,  /* a(7) */
        119, /* a(8) */
        210, /* a(9) */
        289, /* a(10) */
        441, /* a(11) */
        574, /* a(12) */
        804  /* a(13) */
    };

    /* Allocate initial cache if this is the first call. */
    if (cache == NULL) {
        cache_cap = 14;                    /* indices 0..13 inclusive */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failure */

        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = init_vals[i];
        cache_len = 13;                    /* highest computed index */
    }

    /* If the requested n is already cached, return it. */
    if (n <= cache_len)
        return cache[n];

    /* Grow the cache if necessary. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values from cache_len+1 up to n using the recurrence. */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        if (i <= 13) {           /* safety for very small n, though cache already covers this */
            cache[i] = init_vals[i];
            continue;
        }

        seq_t val =
            -cache[i - 1]                           /* -a(n-1) */
            + cache[i - 2]                          /* +a(n-2) */
            + cache[i - 3] * (seq_t)3               /* +3*a(n-3) */
            + cache[i - 4] * (seq_t)3               /* +3*a(n-4) */
            - cache[i - 5]                          /* -a(n-5) */
            - cache[i - 6] * (seq_t)4               /* -4*a(n-6) */
            - cache[i - 7] * (seq_t)4               /* -4*a(n-7) */
            - cache[i - 8]                          /* -a(n-8) */
            + cache[i - 9] * (seq_t)3               /* +3*a(n-9) */
            + cache[i -10] * (seq_t)3               /* +3*a(n-10) */
            + cache[i -11]                          /* +a(n-11) */
            - cache[i -12]                          /* -a(n-12) */
            - cache[i -13];                         /* -a(n-13) */
        cache[i] = val;
    }

    cache_len = (size_t)n;
    return cache[n];
}
