/*
 * A358092  Row sums of the convolution triangle of the Motzkin numbers
 *          (A202710).
 * https://oeis.org/A358092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((36-12*n)*a(n-4) + (30-14*n)*a(n-3) + (3*n-3)*a(n-2) +
 *            (4*n-3)*a(n-1))/n, a(0)=1, a(1)=1, a(2)=3, a(3)=9, a(4)=28
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoisation array */
static idx_t  cache_size = 0;        /* current allocated size */
static idx_t  cache_used  = (idx_t)-1;/* highest index computed */

/* Ensure cache has room for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Initial allocation big enough for first terms. */
        cache_size = (n >= 4 ? n + 1 : 5);
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)28;
        cache_used = 4;
    }
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) exit(1);
        cache = tmp;
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    /* Return cached value if available. */
    if (cache && n <= cache_used) return cache[n];

    /* Build cache up to n. */
    ensure_cache(n);

    /* Compute new values from cache_used+1 up to n. */
    for (idx_t i = (cache_used < (idx_t)4 ? 5 : cache_used + 1); i <= n; ++i) {
        /* Coefficients that can be negative for n>=5; but we use absolute
           values and subtract later to avoid negative intermediary values. */
        idx_t coeff1 = 4*i - 3;   /* (4*n-3)  */
        idx_t coeff2 = 3*i - 3;   /* (3*n-3)  */
        idx_t coeff3 = 14*i - 30; /* (30-14*n) = -(14*n-30) */
        idx_t coeff4 = 12*i - 36; /* (36-12*n) = -(12*n-36) */

        seq_t term1 = (seq_t)coeff1 * cache[i-1];
        seq_t term2 = (seq_t)coeff2 * cache[i-2];
        seq_t term3 = (seq_t)coeff3 * cache[i-3];
        seq_t term4 = (seq_t)coeff4 * cache[i-4];

        /* Numerator = term1 + term2 - term3 - term4; the result is non‑negative. */
        seq_t numerator = term1 + term2 - term3 - term4;

        cache[i] = numerator / (seq_t)i;
    }

    cache_used = n;
    return cache[n];
}
