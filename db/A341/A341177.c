/*
 * A341177  Numbers that when divided by the sum of their digits leave 10 as
 *          remainder.
 * https://oeis.org/A341177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 2062, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A341177: numbers that leave remainder 10 when divided by
   the sum of their decimal digits.  This implementation scans
   successive integers, computes the sum of digits, checks the
   remainder, and caches the sequence values.  */
 
#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* realloc */
 
/* Compute the sum of decimal digits of an idx_t value. */
static seq_t digit_sum_idx(idx_t v)
{
    seq_t sum = 0;
    while (v > 0) {
        sum += (seq_t)(v % 10);
        v /= 10;
    }
    return sum;
}
 
seq_t a(idx_t n)
{
    /* cache[0] stores a(1). */
    static seq_t *cache      = NULL; /* cached sequence values */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t   cache_len = 0;    /* number of elements stored */
    static idx_t   next_candidate = 0; /* next integer to test */
 
    if (n == 0) /* offset is 1; behaviour below offset is undefined */
        return 0;
 
    /* Extend the cache until it holds at least n elements. */
    while (cache_len < n) {
        ++next_candidate;                  /* candidate to test */
 
        seq_t sum = digit_sum_idx(next_candidate);
        if (sum != 0 && (next_candidate % sum) == 10) {
            /* Ensure sufficient capacity. */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) /* allocation failure: propagate zero */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store a valid term (casting to seq_t; overflow is
               intentionally accepted – only values that fit in seq_t
               are required to be correct). */
            cache[cache_len++] = (seq_t)next_candidate;
        }
    }
 
    return cache[(size_t)(n - 1)];
}
