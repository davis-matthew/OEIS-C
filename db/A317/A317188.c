/*
 * A317188  a(n) = 1 + 2 * (a(n-1) + a(n-4) + a(n-6)) + a(n-7) for n>3, with
 *          initial values 0 if n<0, and 1,3,8,18 for n=0..3.
 * https://oeis.org/A317188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 1 + 2 * (a(n-1) + a(n-4) + a(n-6)) + a(n-7),
   with initial values a(0)=1, a(1)=3, a(2)=8, a(3)=18. */

#include <stdlib.h>   /* for malloc, free, NULL */

static seq_t *cache = NULL;       /* memoisation buffer  */
static size_t cache_cap = 0;      /* current capacity in elements   */
static idx_t  ready   = 0;        /* first index that is not yet computed */

static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) > cache_cap) {
#ifdef __cplusplus
        seq_t *tmp = new seq_t[n + 1];          /* allocate construction for arbitrary‑precision types */
#else
        seq_t *tmp = (seq_t *)malloc((n + 1) * sizeof(seq_t));  /* raw allocation for scalar types */
#endif
        if (tmp) {
            if (cache) {
                /* copy already computed values to the resized buffer */
                for (size_t i = 0; i < cache_cap; ++i)
                    tmp[i] = cache[i];
                /* free the old buffer */
#ifdef __cplusplus
                delete[] cache;
#else
                free(cache);
#endif
            }
            cache = tmp;
            cache_cap = (size_t)(n + 1);
        } else {
            /* allocation failed – reset state to avoid corruption */
            cache_cap = 0;
            ready = 0;
        }
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);

    for (idx_t k = ready; k <= n; ++k) {
        seq_t val;
        switch (k) {
        case 0: val = (seq_t)1; break;
        case 1: val = (seq_t)3; break;
        case 2: val = (seq_t)8; break;
        case 3: val = (seq_t)18; break;
        default: {
            seq_t sum = 0;
            if (k >= 1) sum += cache[k - 1];
            if (k >= 4) sum += cache[k - 4];
            if (k >= 6) sum += cache[k - 6];
            val = (seq_t)1 + (seq_t)2 * sum;
            if (k >= 7) val += cache[k - 7];
        }
        }
        cache[k] = val;
    }
    ready = n + 1;
    return cache[n];
}
