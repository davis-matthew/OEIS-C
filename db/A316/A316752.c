/*
 * A316752  Number of nX3 0..1 arrays with every element unequal to 1, 2, 4,
 *          6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence of order 16 with initial values a(1)=1, a(2)=9,
 *            a(3)=15, a(4)=17, a(5)=79, a(6)=185, a(7)=315, a(8)=970,
 *            a(9)=2366, a(10)=4927, a(11)=12747, a(12)=31483, a(13)=71152,
 *            a(14)=174182, a(15)=426352, a(16)=999419, a(17)=2406348
 *            Recurrence: a(n) = 3*a(n-2) +12*a(n-3) +5*a(n-4) -23*a(n-5)
 *            -50...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
/* seq_t and idx_t are declared elsewhere by the build system. */

/* Static cache of previously computed values.  cache[0] is unused. */
static seq_t *cache      = NULL; /* array of values 0 .. cache_cap */
static idx_t   cache_n   = 0;    /* largest index for which cache[i] is valid */
static idx_t   cache_cap = 0;    /* array capacity (index up to cache_cap) */

/* Ensure the cache contains values up to (and including) index n. */
static void
ensure_cache(idx_t n)
{
    if (cache == NULL) { /* first-time allocation */
        /* Allocate enough space for the first 17 values; enlarge as needed. */
        cache_cap = n < 17 ? 17 : n;
        cache = (seq_t *)malloc((size_t)(cache_cap + 1) * sizeof(seq_t));
        if (!cache) abort();              /* unrecoverable memory error */

        /* initialise known values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)17;
        cache[5] = (seq_t)79;
        cache[6] = (seq_t)185;
        cache[7] = (seq_t)315;
        cache[8] = (seq_t)970;
        cache[9] = (seq_t)2366;
        cache[10] = (seq_t)4927;
        cache[11] = (seq_t)12747;
        cache[12] = (seq_t)31483;
        cache[13] = (seq_t)71152;
        cache[14] = (seq_t)174182;
        cache[15] = (seq_t)426352;
        cache[16] = (seq_t)999419;
        cache[17] = (seq_t)2406348;
        cache_n   = 17;
    }

    /* Grow the array if required */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n) new_cap <<= 1; /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)(new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
        /* new elements will be filled in the loop below */
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        if (i <= 17) continue; /* already initialised */
        cache[i] =  3 * cache[i - 2] + 12 * cache[i - 3] + 5 * cache[i - 4]
                  - 23 * cache[i - 5] - 50 * cache[i - 6] - 25 * cache[i - 7]
                  + 40 * cache[i - 8] + 70 * cache[i - 9] + 32 * cache[i -10]
                  - 25 * cache[i -11] - 25 * cache[i -12] - 5 * cache[i -13]
                  + 4 * cache[i -14] - cache[i -15] - 2 * cache[i -16];
    }
    cache_n = n;
}

/* The sequence a(n) as defined above. */
seq_t
a(idx_t n)
{
    if (n < 1)                    /* out of range: nothing defined */
        return (seq_t)0;
    ensure_cache(n);              /* compute up to n if necessary */
    return cache[n];
}
