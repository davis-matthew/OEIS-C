/*
 * A307788  Number of valid hook configurations of permutations of [n] that
 *          avoid the patterns 231 and 321.
 * https://oeis.org/A307788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A307788:
 * Recurrence:
 *   (n+2)*a(n)+2*(-2n-1)*a(n-1)+4*(n-1)*a(n-2)+2*(-2n+5)*a(n-3)+4*(n-4)*a(n-4)=0
 *   a(0)=1,a(1)=1,a(2)=1,a(3)=2,a(4)=5
 * This function memoises the sequence and computes further terms using the recurrence.
 */
#include <stdlib.h>
#include <stdint.h>
/* Note: seq_t and idx_t are defined by the build environment. */

/* static cache for memoised values */
static seq_t *cache = NULL;       /* array of computed a(n) values */
static size_t cache_cap = 0;      /* allocated capacity of the cache */
static idx_t  max_filled = (idx_t)-1;  /* greatest index for which cache contains a value */

/* helper: grow cache to at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* public function -----------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* initialise cache with the first five known values if necessary */
    if (cache == NULL) {
        cache_cap = 5;                     /* indices 0..4 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)5;
        max_filled = 4;
    }

    /* grow cache if the requested index is outside current capacity */
    grow_cache((size_t)n + 1);

    /* if already computed, return result */
    if (n <= max_filled)
        return cache[n];

    /* compute missing terms up to n using the recurrence */
    for (idx_t k = max_filled + 1; k <= n; ++k) {
        /* base cases are already stored for k <= 4 */
        if (k < 5)
            continue;

        /* compute terms of the recurrence using seq_t arithmetic */
        seq_t t1 = ((seq_t)4 * (seq_t)k + (seq_t)2) * cache[k-1];
        seq_t t3 = ((seq_t)4 * (seq_t)(k-1)) * cache[k-2];
        seq_t t2 = ((seq_t)4 * (seq_t)k - (seq_t)10) * cache[k-3];
        seq_t t4 = ((seq_t)4 * (seq_t)(k-4)) * cache[k-4];

        /* numerator = t1 + t2 - t3 - t4 */
        seq_t num = t1 + t2;
        num -= t3;
        num -= t4;

        /* divide by (k+2) to obtain a(k) */
        cache[k] = num / ((seq_t)(k + 2));
    }

    max_filled = n;
    return cache[n];
}
