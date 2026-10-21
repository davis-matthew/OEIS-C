/*
 * A287817  Number of nonary sequences of length n such that no two
 *          consecutive terms have distance 2.
 * https://oeis.org/A287817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - a(n-2) - 23*a(n-3) + 10*a(n-4) + a(n-5),
 *            a(0)=1, a(1)=9, a(2)=67, a(3)=501, a(4)=3747
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* base values */
static const seq_t base[5] = {
    (seq_t)1, (seq_t)9, (seq_t)67, (seq_t)501, (seq_t)3747
};

/* cache state */
static seq_t *cache = NULL;      /* pointer to cached values */
static idx_t cache_cap = 0;      /* allocated capacity (in elements) */
static idx_t last_computed = (idx_t)-1;  /* highest index computed */

/* ensure the cache contains values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* first allocation */
    if (cache == NULL) {
        /* start with a generous size to avoid frequent reallocations */
        cache_cap = 8;
        if (cache_cap < n + 1)
            cache_cap = n + 1;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return;   /* allocation failure – function will return 0 */
        /* store the first five terms */
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = base[i];
        last_computed = 4;
    }

    /* grow capacity if necessary, doubling until big enough */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return;   /* allocation failure – leave cache as is */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any terms that are still missing */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t a5 = cache[i - 5];
            seq_t val = (seq_t)8 * a1
                      - a2
                      - (seq_t)23 * a3
                      + (seq_t)10 * a4
                      + a5;
            cache[i] = val;
        }
        last_computed = n;
    }
}

seq_t a(idx_t n)
{
    if (n < 5)
        return base[n];
    ensure_cache(n);
    if (cache && n < cache_cap)
        return cache[n];
    return (seq_t)0;   /* should never happen */
}
