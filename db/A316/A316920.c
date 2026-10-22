/*
 * A316920  Number of nX3 0..1 arrays with every element unequal to 0, 1, 3,
 *          4, 5, 6 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) -6*a(n-2) +14*a(n-3) -52*a(n-4) +39*a(n-5)
 *            -20*a(n-6) +84*a(n-7) -17*a(n-8) -6*a(n-9) -7*a(n-10)
 *            -50*a(n-11) -16*a(n-12) -47*a(n-13) +144*a(n-14) -64*a(n-15)
 *            -12*a(n-16) +8*a(n-17) with a(1)=3, a(2)=9, a(3)=13, a(4)=31,
 *            a(5)=81, a(6)=184, a(7)=492, a(8)=1457, a(9)=4371, a(10)=1373...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[24] = {
    (seq_t)3,   (seq_t)9,   (seq_t)13,  (seq_t)31,  (seq_t)81,
    (seq_t)184, (seq_t)492, (seq_t)1457,(seq_t)4371,(seq_t)13738,
    (seq_t)44698,(seq_t)147992,(seq_t)497182,(seq_t)1687343,
    (seq_t)5760403,(seq_t)19745581,(seq_t)67869355,(seq_t)233664175,
    (seq_t)805343315,(seq_t)2777680268,(seq_t)9584619952,
    (seq_t)33081967287,(seq_t)114206172731,(seq_t)394310805436
};

static const int coeffs[17] = {
    5,-6,14,-52,39,-20,84,-17,-6,-7,-50,-16,-47,144,-64,-12,8
};

static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* cache_len == size of allocated array */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used by OEIS, but safe */

    /* initialize cache with first 24 terms */
    if (!cache) {
        cache_len = 25;                /* indices 0..24, 0 unused */
        cache = (seq_t *)calloc(25, sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failed */
        for (idx_t i = 1; i <= 24; ++i)
            cache[i] = init[i-1];
    }

    /* if requested term already calculated, return it */
    if (n < cache_len)
        return cache[n];

    /* extend cache to size n+1 (1‑based indexing) */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;    /* allocation failed */
    cache = new_cache;

    /* compute missing terms up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        if (k <= 24) {                 /* should not happen, but safe */
            cache[k] = init[k-1];
        } else {
            seq_t pos = 0;
            seq_t neg = 0;
            /* coefficients correspond to a(k-1) .. a(k-17) */
            for (int t = 0; t < 17; ++t) {
                seq_t term = cache[k-1-t];
                int c = coeffs[t];
                if (c > 0) {
                    pos += term * (seq_t)c;
                } else {
                    neg += term * (seq_t)(-c);
                }
            }
            cache[k] = pos - neg;
        }
    }
    cache_len = n + 1;
    return cache[n];
}
