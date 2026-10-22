/*
 * A362751  Number of total dominating sets in the n-trapezohedral graph.
 * https://oeis.org/A362751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 50*a(n-2) + 66*a(n-3) + 84*a(n-4) -
 *            270*a(n-5) + 63*a(n-6) + 264*a(n-7) - 128*a(n-8) - 72*a(n-9) +
 *            32*a(n-10) Initial values (offset 1): a(1)=4, a(2)=36, a(3)=121,
 *            a(4)=484, a(5)=1764, a(6)=6561, a(7)=24336, a(8)=91204,
 *            a(9)=344569, a(10)=1313316
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* capacity of cache */
static size_t cache_len = 0;   /* number of terms currently stored (n >= 1) */

/* ensure the cache can hold at least 'needed' terms */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* initialise first ten terms (hard‑coded from the sequence definition) */
static void init_base_terms(void)
{
    if (cache_len >= 10)
        return;
    ensure_cache(10);
    cache[0] = (seq_t)4;          /* a(1) */
    cache[1] = (seq_t)36;         /* a(2) */
    cache[2] = (seq_t)121;        /* a(3) */
    cache[3] = (seq_t)484;        /* a(4) */
    cache[4] = (seq_t)1764;       /* a(5) */
    cache[5] = (seq_t)6561;       /* a(6) */
    cache[6] = (seq_t)24336;      /* a(7) */
    cache[7] = (seq_t)91204;      /* a(8) */
    cache[8] = (seq_t)344569;     /* a(9) */
    cache[9] = (seq_t)1313316;    /* a(10) */
    cache_len = 10;
}

/* compute a(n) with memoisation; n >= 1 */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* undefined for n < 1 */

    init_base_terms();

    /* compute needed terms up to n */
    if ((size_t)n > cache_len) {
        ensure_cache((size_t)n);
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            /* recurrence uses indices 1..10 stored at cache[0]..cache[9] */
            seq_t r = 0;
            /* add positive coefficient terms */
            r += cache[i - 1] * (seq_t)12;            /* a(n-1) */
            r += cache[i - 3] * (seq_t)66;            /* a(n-3) */
            r += cache[i - 4] * (seq_t)84;            /* a(n-4) */
            r += cache[i - 6] * (seq_t)63;            /* a(n-6) */
            r += cache[i - 7] * (seq_t)264;           /* a(n-7) */
            r += cache[i -10] * (seq_t)32;            /* a(n-10) */
            /* subtract negative coefficient terms */
            r -= cache[i - 2] * (seq_t)50;            /* a(n-2) */
            r -= cache[i - 5] * (seq_t)270;           /* a(n-5) */
            r -= cache[i - 8] * (seq_t)128;           /* a(n-8) */
            r -= cache[i - 9] * (seq_t)72;            /* a(n-9) */
            cache[i] = r;
        }
        cache_len = (size_t)n;
    }
    return cache[n - 1];
}
