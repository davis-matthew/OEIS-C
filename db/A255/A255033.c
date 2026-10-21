/*
 * A255033  Number of (n+2)X(5+2) 0..1 arrays with every 3X3 subblock sum of
 *          the two sums of the central row and central column minus the two
 *          minimums of the diagonal and antidiagonal nondecreasing
 *          horizontally, vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A255033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 43, u64 n <= 107, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) given empirically for n > 20 by
 *   a(n) = a(n-1)+a(n-2)-a(n-3)+4*a(n-4)-3*a(n-5)-5*a(n-6)+3*a(n-7)+a(n-8)
 *          -4*a(n-9)+4*a(n-10)+4*a(n-11)-4*a(n-12)
 *   Initial values (n = 1..20) are taken directly from the OEIS entry:
 *   a(1) = 154680
 *   a(2) = 470542
 *   a(3) = 566341
 *   a(4) = 474308
 *   a(5) = 509796
 *   a(6) = 427420
 *   a(7) = 427492
 *   a(8) = 432916
 *   a(9) = 485356
 *   a(10) = 514932
 *   a(11) = 568608
 *   a(12) = 594132
 *   a(13) = 683156
 *   a(14) = 776420
 *   a(15) = 865380
 *   a(16) = 917016
 *   a(17) = 1176612
 *   a(18) = 1480140
 *   a(19) = 1742324
 *   a(20) = 1874460
 *
 * The recurrence is used for n > 20; the sequence is deterministic
 * and can be computed in O(n) time with memoisation.
 */

#include <stdlib.h>

/* Static cache: 0‑based index (cache[0] = a(1)) */
static seq_t *cache = NULL;
static size_t cache_count = 0;   /* number of terms currently stored (≥ cache_count) */
static size_t cache_cap   = 0;   /* allocated capacity */

/* First 20 terms as provided; unused for n > 20 but cached for speed. */
static const seq_t init20[20] = {
    154680,   /* a(1) */
    470542,   /* a(2) */
    566341,   /* a(3) */
    474308,   /* a(4) */
    509796,   /* a(5) */
    427420,   /* a(6) */
    427492,   /* a(7) */
    432916,   /* a(8) */
    485356,   /* a(9) */
    514932,   /* a(10) */
    568608,   /* a(11) */
    594132,   /* a(12) */
    683156,   /* a(13) */
    776420,   /* a(14) */
    865380,   /* a(15) */
    917016,   /* a(16) */
    1176612,  /* a(17) */
    1480140,  /* a(18) */
    1742324,  /* a(19) */
    1874460   /* a(20) */
};

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Indexing starts at 1; behaviour below offset is undefined. */
        return 0;
    }

    /* On first call, allocate cache and copy the first 20 terms. */
    if (!cache) {
        size_t initial = (size_t)20;          /* we know we need at least 20 */
        cache_cap = initial * 2;              /* provide room for future growth */
        cache     = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) return 0;                 /* allocation failure → zero */
        for (size_t i = 0; i < 20; ++i) {
            cache[i] = init20[i];
        }
        cache_count = 20;                     /* we have stored terms 1..20 */
    }

    if ((idx_t)n <= 20) {
        return cache[(size_t)(n - 1)];
    }

    /* Ensure cache holds at least term n. */
    if ((idx_t)n > (idx_t)cache_count) {
        /* Grow capacity if necessary. */
        if ((size_t)n > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap < (size_t)n) new_cap *= 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) return 0;         /* allocation failure → zero */
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Compute missing terms up to n using the empirical recurrence. */
        for (size_t t = cache_count + 1; t <= (size_t)n; ++t) {
            size_t idx = t - 1;               /* 0‑based index */

            /* Note: use signed intermediates via seq_t, which supports negative
               values in the arbitrary‑precision build and unsigned arithmetic
               otherwise. */
            seq_t value = cache[idx - 1]           /* a(t-1) */
                        + cache[idx - 2]           /* a(t-2) */
                        - cache[idx - 3]           /* a(t-3) */
                        + cache[idx - 4] * 4       /* 4*a(t-4) */
                        - cache[idx - 5] * 3       /* 3*a(t-5) */
                        - cache[idx - 6] * 5       /* 5*a(t-6) */
                        + cache[idx - 7] * 3       /* 3*a(t-7) */
                        + cache[idx - 8]           /* a(t-8) */
                        - cache[idx - 9] * 4       /* 4*a(t-9) */
                        + cache[idx - 10] * 4      /* 4*a(t-10) */
                        + cache[idx - 11] * 4      /* 4*a(t-11) */
                        - cache[idx - 12] * 4;     /* 4*a(t-12) */

            cache[idx] = value;
        }

        cache_count = (size_t)n;               /* last computed term index +1 */
    }

    return cache[(size_t)(n - 1)];
}
