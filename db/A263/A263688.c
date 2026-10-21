/*
 * A263688  c(n) in (sqrt(2))_n = b(n) + c(n)*sqrt(2), where (x)_n is the
 *          Pochhammer symbol, b(n) and c(n) are integers.
 * https://oeis.org/A263688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of sqrt(2) in the Pochhammer symbol
 *            (sqrt(2))_n Recurrence derived from (sqrt(2))_{k+1} =
 *            (sqrt(2))_k * (k + sqrt(2)): b_{k+1} = b_k * k + 2 * c_k c_{k+1}
 *            = b_k + c_k * k a(n) = c_n. Initial values: a(0)=0, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *b_cache = NULL;   /* coefficients of 1 part */
    static seq_t *c_cache = NULL;   /* coefficients of sqrt(2) part = a(n) */
    static idx_t cached_len = 0;    /* number of values stored (indices 0 .. cached_len-1) */
    static size_t cache_cap = 0;    /* allocated capacity */

    /* initialise cache with a(0) and a(1) */
    if (cached_len == 0) {
        cache_cap   = 2;
        b_cache = seq_alloc_new_array(cache_cap);
        c_cache = seq_alloc_new_array(cache_cap);
        b_cache[0] = (seq_t)1;  /* (sqrt2)_0 = 1 */
        c_cache[0] = (seq_t)0;
        b_cache[1] = (seq_t)0;  /* (sqrt2)_1 = sqrt2 */
        c_cache[1] = (seq_t)1;
        cached_len = 2;
    }

    /* if the requested n is already cached, return it */
    if (n < cached_len)
        return c_cache[n];

    /* grow the cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *new_b = seq_alloc_new_array(new_cap);
        seq_t *new_c = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < cache_cap; ++i) {
            new_b[i] = b_cache[i];
            new_c[i] = c_cache[i];
        }
        free_seq_array(b_cache);
        free_seq_array(c_cache);
        b_cache = new_b;
        c_cache = new_c;
        cache_cap = new_cap;
    }

    /* compute values up to n */
    for (idx_t i = cached_len; i <= n; ++i) {
        idx_t k = i - 1;                /* integer part used in multiplication */
        seq_t b_prev = b_cache[i - 1];
        seq_t c_prev = c_cache[i - 1];

        /* b_next = b_prev * k + 2 * c_prev */
        seq_t b_next = b_prev * ((seq_t)k);
        b_next += ((seq_t)2) * c_prev;

        /* c_next = b_prev + c_prev * k */
        seq_t c_next = b_prev;
        c_next += c_prev * ((seq_t)k);

        b_cache[i] = b_next;
        c_cache[i] = c_next;
    }
    cached_len = n + 1;

    return c_cache[n];
}
