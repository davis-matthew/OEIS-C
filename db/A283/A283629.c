/*
 * A283629  Number of nX3 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly two elements.
 * https://oeis.org/A283629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) -78*a(n-2) +134*a(n-3) +162*a(n-4) -840*a(n-5)
 *            +728*a(n-6) +1398*a(n-7) -3117*a(n-8) +413*a(n-9) +4626*a(n-10)
 *            -4056*a(n-11) -2576*a(n-12) +5376*a(n-13) -768*a(n-14)
 *            -3200*a(n-15) +1536*a(n-16) +768*a(n-17) -512*a(n-18) Initial
 *            values: a(1)=0, a(2)=0, a(3)=37, a(4)=388, a(5)=3729...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Handle offset: sequence is defined for n >= 1; return 0 for n == 0 */
    if (n == 0)
        return 0;

    /* Initialize cache with the first 18 terms (including a(0) = 0) */
    if (!cache) {
        /* Allocate space for indices 0..18 (19 elements) */
        cache_cap = 19;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure: graceful fallback */

        static const seq_t init[19] = {
            /* a(0)  a(1)    a(2)   a(3)   a(4)      a(5)      a(6)      a(7)      a(8)       a(9)         a(10)        a(11)         a(12)          a(13)            a(14)           a(15)           a(16)             a(17)                a(18) */
            (seq_t)0LL,
            (seq_t)0LL,
            (seq_t)0LL,
            (seq_t)37LL,
            (seq_t)388LL,
            (seq_t)3729LL,
            (seq_t)28828LL,
            (seq_t)203025LL,
            (seq_t)1325980LL,
            (seq_t)8216341LL,
            (seq_t)48912768LL,
            (seq_t)282293033LL,
            (seq_t)1589466368LL,
            (seq_t)8771503165LL,
            (seq_t)47606031352LL,
            (seq_t)254776347029LL,
            (seq_t)1347286728096LL,
            (seq_t)7051427209337LL,
            (seq_t)36575153581764LL
        };
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = init[i];
        cache_len = 18; /* last computed index */
    }

    /* If already computed, return promptly */
    if (n <= cache_len)
        return cache[n];

    /* Ensure capacity for indices up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure: graceful fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;

        /* Positive coefficients */
        pos += (seq_t)15   * cache[i - 1];
        pos += (seq_t)134  * cache[i - 3];
        pos += (seq_t)162  * cache[i - 4];
        pos += (seq_t)728  * cache[i - 6];
        pos += (seq_t)1398 * cache[i - 7];
        pos += (seq_t)413  * cache[i - 9];
        pos += (seq_t)4626 * cache[i - 10];
        pos += (seq_t)5376 * cache[i - 13];
        pos += (seq_t)1536 * cache[i - 16];
        pos += (seq_t)768  * cache[i - 17];

        /* Negative coefficients (subtracted later) */
        neg += (seq_t)78   * cache[i - 2];
        neg += (seq_t)840  * cache[i - 5];
        neg += (seq_t)3117 * cache[i - 8];
        neg += (seq_t)4056 * cache[i - 11];
        neg += (seq_t)2576 * cache[i - 12];
        neg += (seq_t)768  * cache[i - 14];
        neg += (seq_t)3200 * cache[i - 15];
        neg += (seq_t)512  * cache[i - 18];

        cache[i] = pos - neg;
    }
    cache_len = n;
    return cache[n];
}
