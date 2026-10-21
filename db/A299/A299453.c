/*
 * A299453  Number of nX3 0..1 arrays with every element equal to 0, 2, 3, 5,
 *          6 or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) -8*a(n-3) -13*a(n-4) +20*a(n-5) +21*a(n-6)
 *            +4*a(n-7) -30*a(n-8) -44*a(n-9) -17*a(n-10)
 *            +72*a(n-11)+116*a(n-12)-20*a(n-13)-72*a(n-14)
 *            -52*a(n-15)-12*a(n-16)+24*a(n-17)+8*a(n-18) for n>19. Initial
 *            values (a(1) .. a(19)):
 *            1,7,7,19,35,95,223,571,1535,4091,11247,31195,86931,244295,
 *            688...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t base[20] = {
    0,
    1,          /* a(1) */
    7,          /* a(2) */
    7,          /* a(3) */
    19,         /* a(4) */
    35,         /* a(5) */
    95,         /* a(6) */
    223,        /* a(7) */
    571,        /* a(8) */
    1535,       /* a(9) */
    4091,       /* a(10) */
    11247,      /* a(11) */
    31195,      /* a(12) */
    86931,      /* a(13) */
    244295,     /* a(14) */
    688227,     /* a(15) */
    1944479,    /* a(16) */
    5504723,    /* a(17) */
    15600559,   /* a(18) */
    44253807    /* a(19) */
};

static seq_t *cache = NULL;       /* allocated cache for a(n) */
static idx_t  cache_cap = 0;      /* capacity of cache (indexable up to cache_cap-1) */

static void
grow_cache(idx_t needed)
{
    if (cache_cap > needed) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 20;
    while (new_cap <= needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    cache = tmp;
    /* initialise new entries to 0 */
    memset(cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache_cap = new_cap;
    /* fill the first 19 entries with known values */
    for (idx_t i = 1; i <= 19 && i < cache_cap; ++i) cache[i] = base[i];
}

seq_t a(idx_t n)
{
    if (n <= 0) return 0;   /* undefined for n <= 0, return 0 */
    if (n <= 19) return base[n];

    /* ensure cache large enough */
    grow_cache(n);

    /* compute missing terms up to n */
    /* we keep track of last computed index */
    static idx_t last = 0;
    if (last == 0) last = 19;   /* initial known maximum */

    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        pos += 4 * cache[i-1];
        pos += 20 * cache[i-5];
        pos += 21 * cache[i-6];
        pos += 4 * cache[i-7];
        pos += 72 * cache[i-11];
        pos += 116 * cache[i-12];
        pos += 24 * cache[i-17];
        pos += 8 * cache[i-18];

        neg += 8 * cache[i-3];
        neg += 13 * cache[i-4];
        neg += 30 * cache[i-8];
        neg += 44 * cache[i-9];
        neg += 17 * cache[i-10];
        neg += 20 * cache[i-13];
        neg += 72 * cache[i-14];
        neg += 52 * cache[i-15];
        neg += 12 * cache[i-16];

        cache[i] = pos - neg;
    }
    last = n;
    return cache[n];
}
