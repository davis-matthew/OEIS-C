/*
 * A298082  Number of nX3 0..1 arrays with every element equal to 2, 3, 5 or 6
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 25, u64 n <= 47, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) satisfies the linear recurrence
 * for n > 17:
 *   a(n) = 3*a(n-1) + 2*a(n-2) - 4*a(n-3) - 11*a(n-4)
 *          + a(n-5) + 15*a(n-6) + 7*a(n-7) + a(n-8)
 *          - 24*a(n-9) - 39*a(n-10) + 13*a(n-11) + 32*a(n-12)
 *          + 8*a(n-13) + 5*a(n-14) - 2*a(n-15) - 2*a(n-16)
 * Initial values up to n = 17:
 *   a(1)=0  a(2)=3  a(3)=0  a(4)=3  a(5)=1
 *   a(6)=7  a(7)=18 a(8)=52 a(9)=144 a(10)=405
 *   a(11)=1124 a(12)=3113 a(13)=8688 a(14)=24346
 *   a(15)=68385 a(16)=192524 a(17)=542537
 *
 * The implementation memoises the sequence and grows the
 * cache dynamically.  It works for all integer sizes
 * supported by seq_t and accepts any order of calls.
 */

#include <stdlib.h>

/* Memoisation cache */
static seq_t *cache = NULL;           /* array of computed terms, 1‑based */
static size_t cache_len = 0;          /* highest index cached so far */
static size_t cache_cap = 0;          /* allocated capacity */

/* Ensure that a(0 .. n) are computed and cached */
static void ensure_cache(idx_t n)
{
    /* Convert to size_t for internal use */
    size_t up_to = (size_t)n;

    /* If already computed, nothing to do */
    if (up_to <= cache_len)
        return;

    /* Allocate or grow the cache */
    if (cache == NULL) {
        cache_cap = 18;                     /* enough for initial terms */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return;                         /* allocation failure – caller gets garbage */
    }

    if (up_to + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= up_to)
            new_cap <<= 1;                  /* double until big enough */
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (p == NULL)
            return;                         /* allocation failure */
        cache = p;
        cache_cap = new_cap;
    }

    /* Initial constants (index 0 unused) */
    static const seq_t init[18] = {
        0,          /* dummy for index 0 */
        0, 3, 0, 3, 1,          /* a(1) .. a(5) */
        7, 18, 52, 144, 405,    /* a(6) .. a(10) */
        1124, 3113, 8688, 24346, /* a(11)..a(14) */
        68385, 192524, 542537     /* a(15)..a(17) */
    };

    /* Initialise base values if needed */
    if (cache_len < 5) {
        for (size_t i = 1; i <= 5; ++i)
            cache[i] = init[i];
        cache_len = 5;
    }

    /* Compute missing terms up to up_to */
    for (size_t i = cache_len + 1; i <= up_to; ++i) {
        seq_t v;
        if (i <= 17) {
            v = init[i];
        } else {
            /* apply recurrence */
            v = 3 * cache[i-1]
              + 2 * cache[i-2]
              - 4 * cache[i-3]
              - 11 * cache[i-4]
              + cache[i-5]
              + 15 * cache[i-6]
              + 7  * cache[i-7]
              + cache[i-8]
              - 24 * cache[i-9]
              - 39 * cache[i-10]
              + 13 * cache[i-11]
              + 32 * cache[i-12]
              + 8  * cache[i-13]
              + 5  * cache[i-14]
              - 2  * cache[i-15]
              - 2  * cache[i-16];
        }
        cache[i] = v;
    }
    cache_len = up_to;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* behaviour for n < 1 is irrelevant */

    ensure_cache(n);
    return cache[(size_t)n];
}
