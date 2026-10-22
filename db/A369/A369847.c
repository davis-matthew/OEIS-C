/*
 * A369847  Number of compositions of 5*n-2 into parts 3 and 5.
 * https://oeis.org/A369847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A052920(5*n-2) = number of compositions of 5*n-2 into
 *            parts 3 and 5 Recurrence for n>5:
 *            a(n)=5*a(n-1)-10*a(n-2)+11*a(n-3)-5*a(n-4)+a(n-5) Initial
 *            values: a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=12
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;      /* cache[0] unused; cache[i] holds a(i) */
static size_t cache_size = 0;    /* number of allocated slots (including index 0) */

/* ensure cache is large enough up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cache_size >= need)
        return;

    seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
    if (!new_cache)          /* allocation failed – fallback to local calculation */
        return;
    /* initialise new slots to zero (old slots preserved) */
    for (size_t i = cache_size; i < need; ++i)
        new_cache[i] = 0;
    cache      = new_cache;
    cache_size = need;
}

seq_t a(idx_t n)
{
    if (n < 1)  /* behaviour below offset irrelevant */
        return 0;

    if (n <= 5) {               /* return known initial values directly */
        switch ((idx_t)n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)3;
            case 4: return (seq_t)5;
            case 5: return (seq_t)12;
        }
    }

    /* ensure we have space for all indices up to n */
    ensure_cache(n);

    /* initialise the base cases in the cache */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)3;
    cache[4] = (seq_t)5;
    cache[5] = (seq_t)12;

    /* compute missing values using the linear recurrence */
    /* start from the first index that may need to be recomputed */
    for (size_t i = 6; i <= (size_t)n; ++i) {
        seq_t v1 = cache[i - 1];
        seq_t v2 = cache[i - 2];
        seq_t v3 = cache[i - 3];
        seq_t v4 = cache[i - 4];
        seq_t v5 = cache[i - 5];

        seq_t val = v1 * (seq_t)5;
        val += v5;
        val += v3 * (seq_t)11;
        val -= v2 * (seq_t)10;
        val -= v4 * (seq_t)5;

        cache[i] = val;
    }

    return cache[n];
}
