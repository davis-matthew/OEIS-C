/*
 * A379182  Number of minimal edge covers in the n-double cone graph.
 * https://oeis.org/A379182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-3) - 21*a(n-4) + 8*a(n-5) + 8*a(n-6) +
 *            34*a(n-7) - 12*a(n-8) + 20*a(n-9) - 28*a(n-10) + 6*a(n-11) -
 *            24*a(n-12) + 20*a(n-13) - 13*a(n-14) + 12*a(n-15) - 4*a(n-16) +
 *            2*a(n-17) - a(n-18); a(0) = 0, a(1) = 1, a(2) = 21, a(3) = 58,
 *            a(4) = 149, a(5) = 566, a(6) = 1676, a(7) = 54...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial 19 values */
    static const seq_t init[19] = {
        0ULL,          /* a(0) */
        1ULL,          /* a(1) */
        21ULL,         /* a(2) */
        58ULL,         /* a(3) */
        149ULL,        /* a(4) */
        566ULL,        /* a(5) */
        1676ULL,       /* a(6) */
        5482ULL,       /* a(7) */
        18021ULL,      /* a(8) */
        59665ULL,      /* a(9) */
        199700ULL,     /* a(10) */
        670517ULL,     /* a(11) */
        2259384ULL,    /* a(12) */
        7624878ULL,    /* a(13) */
        25759564ULL,   /* a(14) */
        87078065ULL,   /* a(15) */
        294452965ULL,  /* a(16) */
        995889190ULL,  /* a(17) */
        3368616437ULL  /* a(18) */
    };

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* If requested index already cached, return immediately */
    if (n < cache_len)
        return cache[(size_t)n];

    /* Initialise cache with known values */
    if (cache_len == 0) {
        cache_len = 19;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = init[i];
    }

    /* Grow cache up to requested index */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) { free(cache); abort(); }
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute values beyond the stored initial block */
    for (idx_t i = 19; i <= n; ++i) {
        /* Positive contributions */
        seq_t pos = 0;
        pos += 4ULL  * cache[(size_t)(i - 1)];
        pos += 8ULL  * cache[(size_t)(i - 5)];
        pos += 8ULL  * cache[(size_t)(i - 6)];
        pos += 34ULL * cache[(size_t)(i - 7)];
        pos += 20ULL * cache[(size_t)(i - 9)];
        pos += 6ULL  * cache[(size_t)(i - 11)];
        pos += 20ULL * cache[(size_t)(i - 13)];
        pos += 12ULL * cache[(size_t)(i - 15)];
        pos += 2ULL  * cache[(size_t)(i - 17)];

        /* Negative contributions */
        seq_t sub = 0;
        sub += 2ULL  * cache[(size_t)(i - 3)];
        sub += 21ULL * cache[(size_t)(i - 4)];
        sub += 12ULL * cache[(size_t)(i - 8)];
        sub += 28ULL * cache[(size_t)(i - 10)];
        sub += 24ULL * cache[(size_t)(i - 12)];
        sub += 13ULL * cache[(size_t)(i - 14)];
        sub += 4ULL  * cache[(size_t)(i - 16)];
        sub += 1ULL  * cache[(size_t)(i - 18)];

        cache[(size_t)i] = pos - sub;
    }

    return cache[(size_t)n];
}
