/*
 * A374336  a(n) is the numerator of x(n) = (2*x(n-1) + c(n)) mod 1, where
 *          c(n) = 1/n if n is a power of 3 and 0 otherwise, with x(0) = 0.
 * https://oeis.org/A374336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 728, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the numerator of the fraction x(n) defined by:  
 *  x(0) = 0
 *  x(n) = ( 2 * x(n‑1) + c(n) ) mod 1
 *  c(n) = 1 / n  when n is a power of 3, 0 otherwise.
 *  The returned value a(n) is the numerator of the reduced fraction
 *  representation of x(n).  The sequence starts with a(0) = 0.  */

/* BEWARE:  Requires the types seq_t and idx_t to be available in
 * the build environment.  No other headers are needed.            */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;          /* a[0..last_index]  */
    static idx_t cache_len = 0;          /* number of allocated slots */
    static seq_t last_a = 0;             /* a of the last computed index */
    static idx_t last_d = 1;             /* denominator at the last index (as idx_t) */
    static idx_t last_index = 0;         /* the index of the last computed term */

    /* Initialisation of the cache */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = 0;
        last_a = 0;
        last_d = 1;
        last_index = 0;
    }

    /* If the requested term is already cached, return it immediately */
    if (n < cache_len)
        return cache[n];

    /* Extend the cache to hold terms up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (new_cache == NULL)
        return 0; /* allocation failed – return 0 as a safe value */
    cache = new_cache;

    /* Generate the missing terms one by one */
    for (idx_t i = last_index + 1; i <= n; ++i) {
        /* Test whether i is a power of 3 */
        idx_t m = i;
        int is_pow3 = 0;
        if (m > 0) {
            while (m % 3 == 0)
                m /= 3;
            is_pow3 = (m == 1);
        }

        if (is_pow3) {
            /* New denominator becomes i itself */
            last_d = i;
            /* a_new = (6 * a_old + 1) mod last_d */
            idx_t tmp = ((idx_t)6 * (idx_t)last_a + 1) % last_d;
            last_a = (seq_t)tmp;
        } else {
            /* Denominator stays the same */
            idx_t tmp = ((idx_t)2 * (idx_t)last_a) % last_d;
            last_a = (seq_t)tmp;
        }

        cache[i] = last_a;
    }

    /* Update bookkeeping information */
    last_index = n;
    cache_len = n + 1;

    return cache[n];
}
