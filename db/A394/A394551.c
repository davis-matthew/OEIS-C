/*
 * A394551  a(n) = n! * [x^n] exp(-Li_{-2}(x)), where Li_{-2}(x) =
 *          x*(1+x)/(1-x)^3 is the polylogarithm of order -2.
 * https://oeis.org/A394551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! * [x^n] exp(-Li_{-2}(x)) Recurrence: a(0)=1 ; for n≥1
 *            a(n) = -∑_{k=1..n} k³ · a(n-k) · ∏_{t=n-k+1}^{n-1} t (the
 *            product is 1 when the lower bound exceeds the upper bound)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 17, i128 n <= 29,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"

static seq_t *cache = NULL;        /* cached a(n) values  */
static idx_t cache_size = 0;       /* allocated capacity   */
static idx_t cache_len  = 0;       /* number of values stored */

/* enlarge the cache so that it can hold values up to index n */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;  /* already available */

    /* grow storage if necessary */
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        if (new_size < 2) new_size = 2;
        while (new_size <= n) new_size <<= 1;
        /* allocate new array */
        seq_t *new_cache = seq_alloc_new_array(new_size);
        /* copy existing values */
        for (idx_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        /* keep old memory (leak is harmless for this programmer‑task) */
        cache = new_cache;
        cache_size = new_size;
    }

    /* compute missing values */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {                     /* base case */
            cache[0] = 1;
            continue;
        }
        seq_t sum = 0;
        seq_t prod = 1;                    /* ∏_{t=i-j+1}^{i-1} t  */
        for (idx_t j = 1; j <= i; ++j) {
            if (j > 1) {                   /* update product for next j */
                prod = prod * (seq_t)(i - j + 1);
            }
            seq_t j3 = (seq_t)j * j * j;   /* j³  */
            seq_t term = j3 * prod * cache[i - j];
            sum -= term;                   /* accumulate the negative term */
        }
        cache[i] = sum;  /* already equals –∑ …  */
    }

    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    if (n < cache_len) return cache[n];
    ensure_cache(n);
    return cache[n];
}
