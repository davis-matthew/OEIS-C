/*
 * A294318  a(n) = Product_{k=0..n} (3*k + 1)!.
 * https://oeis.org/A294318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prod_{k=0..n} (3*k + 1)!
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* a(0) .. a(k) */
static size_t cache_len = 0;          /* number of elements in cache */
static seq_t last_factorial = 0;      /* factorial of (3*last_k + 1)! */
static idx_t  last_k = 0;             /* last k computed */

/* a(n) = product_{k=0..n} (3*k + 1)! */
seq_t a(idx_t n)
{
    /* return cached value if it exists */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* resize cache to hold terms up to n */
    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        return 0;   /* allocation failed */
    cache = new_cache;

    seq_t prod;
    seq_t factorial;
    idx_t k;

    if (cache_len == 0) {
        /* initialise for k = 0: (3*0+1)! = 1! = 1 */
        factorial = (seq_t)1;
        prod      = factorial;
        cache[0]  = prod;
        last_k    = 0;
        last_factorial = factorial;
        cache_len = 1;
    } else {
        /* start from the last cached value */
        prod      = cache[cache_len - 1];
        factorial = last_factorial;
        k         = cache_len - 1;
    }

    /* compute subsequent terms */
    for (k = last_k + 1; k <= n; ++k) {
        /* current m = 3*k + 1 */
        idx_t m = 3 * k + 1;
        /* previous m = 3*(k-1) + 1 */
        idx_t prev_m = 3 * (k - 1) + 1;
        /* extend factorial from (prev_m)! to m! by multiplying the 3 missing factors */
        for (idx_t t = prev_m + 1; t <= m; ++t)
            factorial = factorial * (seq_t)t;
        /* update product */
        prod = prod * factorial;
        cache[(size_t)k] = prod;
    }

    /* update cache metadata */
    cache_len   = new_len;
    last_k      = n;
    last_factorial = factorial;

    return cache[(size_t)n];
}
