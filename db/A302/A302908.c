/*
 * A302908  Expansion of e.g.f. 6*exp(-x)/(6 + x*(3 + x)*(6 + x)).
 * https://oeis.org/A302908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   Recurrence 6*a(n) + 6*(3*n+1)*a(n-1) + 9*(n+2)*(n-1)*a(n-2) +
 *            (n-1)*(n-2)*(n+9)*a(n-3) + (n-1)*(n-2)*(n-3)*a(n-4) = 0; initial
 *            values: a(0)=1, a(1)=-4, a(2)=22, a(3)=-164
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq.h"

/* external types: seq_t, idx_t */
seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0;
    static idx_t computed_up_to = 0;   /* largest index for which cache[i] is valid */

    /* grow cache if necessary */
    if (n >= cache_capacity) {
        idx_t new_capacity = n + 1;
        seq_t *new_cache = alloc_seq_array(new_capacity);
        /* copy existing values if any */
        if (cache != NULL) {
            for (idx_t i = 0; i < cache_capacity; ++i)
                new_cache[i] = cache[i];
            free_seq(cache);
        }
        cache = new_cache;
        cache_capacity = new_capacity;

        /* initialise base values up to index 3 if they exist in the new cache */
        if (cache_capacity > 0) cache[0] = (seq_t)1;
        if (cache_capacity > 1) cache[1] = (seq_t)(-4);
        if (cache_capacity > 2) cache[2] = (seq_t)22;
        if (cache_capacity > 3) cache[3] = (seq_t)(-164);

        /* adjust computed_up_to after growth */
        if (computed_up_to < 3) {
            /* we just initialised up to min(3, cache_capacity-1) */
            computed_up_to = cache_capacity > 3 ? 3 : cache_capacity - 1;
        }
    }

    /* if we already have the requested value, return it */
    if (n <= computed_up_to)
        return cache[n];

    /* ensure base values (if they were not set by above growth) */
    if (computed_up_to < 3) {
        cache[0] = (seq_t)1;
        if (cache_capacity > 1) cache[1] = (seq_t)(-4);
        if (cache_capacity > 2) cache[2] = (seq_t)22;
        if (cache_capacity > 3) cache[3] = (seq_t)(-164);
        computed_up_to = cache_capacity > 3 ? 3 : cache_capacity - 1;
    }

    /* compute terms from computed_up_to+1 up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        if (i <= 3) {
            /* base terms already initialised */
            continue;
        }
        /* compute using the recurrence
           a(i) = - (6*(3*i+1)*a(i-1) + 9*(i+2)*(i-1)*a(i-2)
                     + (i-1)*(i-2)*(i+9)*a(i-3)
                     + (i-1)*(i-2)*(i-3)*a(i-4)) / 6
        */
        seq_t k1 = (seq_t)6 * ((seq_t)3 * i + (seq_t)1);
        seq_t term1 = k1 * cache[i - 1];

        seq_t k2 = (seq_t)9 * ((seq_t)i + (seq_t)2) * ((seq_t)i - (seq_t)1);
        seq_t term2 = k2 * cache[i - 2];

        seq_t k3 = ((seq_t)i - (seq_t)1) * ((seq_t)i - (seq_t)2) * ((seq_t)i + (seq_t)9);
        seq_t term3 = k3 * cache[i - 3];

        seq_t k4 = ((seq_t)i - (seq_t)1) * ((seq_t)i - (seq_t)2) * ((seq_t)i - (seq_t)3);
        seq_t term4 = k4 * cache[i - 4];

        seq_t sum   = term1 + term2 + term3 + term4;
        seq_t value = - (sum / (seq_t)6);
        cache[i] = value;
    }
    computed_up_to = n;
    return cache[n];
}
