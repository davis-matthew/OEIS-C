/*
 * A297064  Number of maximal matchings in the n-dipyramidal graph.
 * https://oeis.org/A297064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 21, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 5*a(n-2) + 3*a(n-3) - 10*a(n-4) - 12*a(n-5) + 7*a(n-6)
 *              + 18*a(n-7) + 4*a(n-8) - 11*a(n-9) - 8*a(n-10)
 *              + a(n-11) + 3*a(n-12) + a(n-13)
 * Initial values:
 * a(1)=2, a(2)=4, a(3)=12, a(4)=14, a(5)=40, a(6)=56, a(7)=112, a(8)=178,
 * a(9)=306, a(10)=482, a(11)=792, a(12)=1214, a(13)=1924.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* forward declaration of helper to grow the cache */
static void grow_cache(size_t *len, seq_t **ptr, size_t needed);

/* add coefficient * val to sum */
static inline void add_scaled(seq_t *sum, seq_t val, int coeff)
{
    *sum += val * (seq_t)coeff;
}

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* allocated length (in entries)   */
    static idx_t max_computed = 0;    /* highest n computed so far       */

    /* initialise cache once with the first 13 terms */
    if (cache == NULL) {
        cache_len = 14;     /* indices 0..13, index 0 unused */
        /* allocate array */
#ifdef __cplusplus
        cache = new seq_t[cache_len];
#else
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
#endif
        /* sentinel for index 0 (not used) */
        cache[0] = 0;
        /* initial values from the OEIS entry */
        cache[1]  = 2;
        cache[2]  = 4;
        cache[3]  = 12;
        cache[4]  = 14;
        cache[5]  = 40;
        cache[6]  = 56;
        cache[7]  = 112;
        cache[8]  = 178;
        cache[9]  = 306;
        cache[10] = 482;
        cache[11] = 792;
        cache[12] = 1214;
        cache[13] = 1924;
        max_computed = 13;
    }

    /* if requested n already computed, return it */
    if (n <= max_computed) {
        return cache[n];
    }

    /* grow cache storage if necessary */
    if ((idx_t)(cache_len - 1) < n) {
        size_t needed = (size_t)(n + 1);
        grow_cache(&cache_len, &cache, needed);
    }

    /* compute missing values iteratively */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t pos = 0;
        seq_t neg = 0;

        add_scaled(&pos, cache[k - 2], 5);
        add_scaled(&pos, cache[k - 3], 3);
        add_scaled(&neg, cache[k - 4], 10);
        add_scaled(&neg, cache[k - 5], 12);
        add_scaled(&pos, cache[k - 6], 7);
        add_scaled(&pos, cache[k - 7], 18);
        add_scaled(&pos, cache[k - 8], 4);
        add_scaled(&neg, cache[k - 9], 11);
        add_scaled(&neg, cache[k - 10], 8);
        add_scaled(&pos, cache[k - 11], 1);
        add_scaled(&pos, cache[k - 12], 3);
        add_scaled(&pos, cache[k - 13], 1);

        cache[k] = pos - neg;   /* pos >= neg for this sequence */
    }

    max_computed = n;
    return cache[n];
}

/* utility: grow an array of seq_t values to 'needed' size */
static void grow_cache(size_t *len, seq_t **ptr, size_t needed)
{
    size_t new_len = *len;
    if (new_len == 0) new_len = 1;
    while (new_len < needed) {
        new_len *= 2;
    }
    if (new_len == *len) return;  /* already large enough */

#ifdef __cplusplus
    seq_t *newp = new seq_t[new_len];
    for (size_t i = 0; i < *len; ++i) newp[i] = (*ptr)[i];
    delete[] *ptr;
#else
    seq_t *newp = (seq_t *)realloc(*ptr, new_len * sizeof(seq_t));
#endif
    *ptr = newp;
    *len = new_len;
}
