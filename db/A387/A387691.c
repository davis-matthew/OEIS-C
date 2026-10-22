/*
 * A387691  a(n) = Sum_{k=0..floor(n/4)} 2^(n-4*k) * binomial(2*n-6*k,2*k).
 * https://oeis.org/A387691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} 2^(n-4*k) * binomial(2*n-6*k,2*k)
 *            The recurrence a(n) = 4*a(n-1) - 4*a(n-2) + 2*a(n-4) + 4*a(n-5)
 *            - a(n-8) with a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=17, a(5)=44,
 *            a(6)=124, a(7)=352, a(8)=977
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cached sequence values.  */
static seq_t *cache = NULL;          /* Pointer to allocated array. */
static idx_t cache_len = 0;          /* Number of values stored (valid indices 0..cache_len-1). */

/* Base values for n = 0 .. 8. */
static const seq_t base_vals[9] = {
    (seq_t)1,
    (seq_t)2,
    (seq_t)4,
    (seq_t)8,
    (seq_t)17,
    (seq_t)44,
    (seq_t)124,
    (seq_t)352,
    (seq_t)977
};

/* Ensure the cache contains values up to index n.  */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;                 /* Already computed. */

    /* Grow the cache to accommodate indices 0..n. */
    idx_t new_len = n + 1;
    if (cache_len == 0) {
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    }
    if (!cache) {
        /* Allocation failure – abort program. */
        abort();
    }

    /* If we don't have enough base values yet, copy them. */
    if (cache_len < 9) {
        idx_t limit = (cache_len < 9) ? 9 : cache_len;
        for (idx_t i = cache_len; i < limit && i <= n; ++i)
            cache[i] = base_vals[i];
        cache_len = (limit > cache_len) ? limit : cache_len;
    }

    /* Compute remaining values using the recurrence.  */
    for (idx_t i = (cache_len > 8 ? cache_len : 9); i <= n; ++i) {
        seq_t term = ((seq_t)4 * cache[i-1])       /* 4*a(i-1) */
                  + ((seq_t)2 * cache[i-4])       /* +2*a(i-4) */
                  + ((seq_t)4 * cache[i-5]);      /* +4*a(i-5) */

        term -= ((seq_t)4 * cache[i-2]);           /* -4*a(i-2) */
        term -= cache[i-8];                       /* -a(i-8)    */

        cache[i] = term;
    }
    cache_len = new_len;
}

/* Public sequence function.  */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
