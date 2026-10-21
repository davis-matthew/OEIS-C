/*
 * A251025  Number of (n+1)X(5+1) 0..3 arrays with no 2X2 subblock having the
 *          sum of its diagonal elements greater than the maximum of its
 *          antidiagonal elements.
 * https://oeis.org/A251025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 16, u64 n <= 43, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(n) = 8*a(n-1) -25*a(n-2) +35*a(n-3) -7*a(n-4) -49*a(n-5) +
 *          77*a(n-6) -55*a(n-7) +20*a(n-8) -3*a(n-9)      for n > 14
 * Initial values:
 *   a(1)=18330, a(2)=60044, a(3)=118871, a(4)=257578, a(5)=516684,
 *   a(6)=1078862, a(7)=2203974, a(8)=4638560, a(9)=9823382,
 *   a(10)=21323795, a(11)=46876790, a(12)=104722539, a(13)=236069897,
 *   a(14)=536548809.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n <= 0 return 0. */
    if (n == 0) return (seq_t)0;

    /* Static initial terms.  Use a fixed size array so that the
       initializer is a compile‑time constant even when seq_t is small
       (e.g. uint8_t).  The literal values are cast to seq_t to
       guarantee the conversions. */
    static const seq_t init_arr[14] = {
        (seq_t)18330ULL,   (seq_t)60044ULL,    (seq_t)118871ULL,
        (seq_t)257578ULL,  (seq_t)516684ULL,   (seq_t)1078862ULL,
        (seq_t)2203974ULL, (seq_t)4638560ULL,  (seq_t)9823382ULL,
        (seq_t)21323795ULL,(seq_t)46876790ULL, (seq_t)104722539ULL,
        (seq_t)236069897ULL,(seq_t)536548809ULL
    };

    /* Return a pre‑computed initial value directly. */
    if (n <= 14) return init_arr[n - 1];

    /* Memoisation cache.  The cache stores a(1) at index 0,
       a(2) at index 1, etc. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* highest index currently stored (1‑based) */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* Initialise the cache with the 14 known initial terms. */
    if (cache == NULL) {
        cache_cap = 14;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 14; ++i) cache[i] = init_arr[i];
        cache_len = 14;
    }

    /* Grow the cache if the requested n is larger than what we have. */
    if (n > cache_len) {
        /* Increase capacity to at least n. */
        idx_t new_cap = cache_cap;
        while (new_cap < n) new_cap <<= 1;
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;            /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute terms from cache_len+1 up to n using the recurrence. */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* a(i-k) resides at cache[(i-k)-1] = cache[i-k-1] */
            seq_t a1 = cache[i - 2];   /* a(i-1) */
            seq_t a2 = cache[i - 3];   /* a(i-2) */
            seq_t a3 = cache[i - 4];   /* a(i-3) */
            seq_t a4 = cache[i - 5];   /* a(i-4) */
            seq_t a5 = cache[i - 6];   /* a(i-5) */
            seq_t a6 = cache[i - 7];   /* a(i-6) */
            seq_t a7 = cache[i - 8];   /* a(i-7) */
            seq_t a8 = cache[i - 9];   /* a(i-8) */
            seq_t a9 = cache[i -10];   /* a(i-9) */

            seq_t term = a1 * (seq_t)8
                       - a2 * (seq_t)25
                       + a3 * (seq_t)35
                       - a4 * (seq_t)7
                       - a5 * (seq_t)49
                       + a6 * (seq_t)77
                       - a7 * (seq_t)55
                       + a8 * (seq_t)20
                       - a9 * (seq_t)3;

            cache[i - 1] = term;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
