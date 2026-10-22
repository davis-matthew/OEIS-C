/*
 * A305747  Let c be the n-th composite number; then a(n) is the smallest
 *          divisor of c such that a(n) >= sqrt(c).
 * https://oeis.org/A305747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A033677(A002808(n))
 * Range:     u8 n <= 44, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_prime, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(n) = smallest divisor d of the n-th composite
 *          such that d >= sqrt(c), where c is that composite. */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/is_prime.h"
#include "../utils/floor_sqrt.h"

/* Return the smallest divisor of the composite number c that is
   greater than or equal to sqrt(c).  c is assumed to be composite
   and >= 4. */
static seq_t compute_a_composite(idx_t c)
{
    seq_t cs = (seq_t)c;
    seq_t sqrt_floor = floor_sqrt(cs);
    for (idx_t d = (idx_t)sqrt_floor; d >= (idx_t)2; --d) {
        if (c % d == 0) {
            return (seq_t)c / (seq_t)d;   /* divisor >= sqrt(c) */
        }
    }
    /* Should never happen for a composite number. */
    return (seq_t)c;
}

/* Cache of previously computed terms. */
static seq_t *cache = NULL;      /* array of a(1) .. a(cache_cap) */
static idx_t   cache_cap = 0;    /* number of entries stored */
static idx_t   last_comp_val = 1;/* last composite processed (init before 4) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for offset 0 */

    if (n <= cache_cap) {
        return cache[n - 1];      /* existing entry */
    }

    /* Preserve the old capacity before replacing the array. */
    idx_t prev_cap = cache_cap;

    /* Allocate a larger array and copy existing values. */
    seq_t *new_cache = seq_alloc_new_array(n);
    for (idx_t i = 0; i < prev_cap; ++i) {
        new_cache[i] = cache[i];
    }
    if (cache != NULL) {
        seq_free_array(cache);
    }
    cache = new_cache;
    cache_cap = n;

    /* Generate composites from after the last processed one
     * until the cache is full. */
    for (idx_t test = last_comp_val + 1, idx = prev_cap;
         idx < cache_cap; ++test) {
        if (!is_prime(test)) {
            new_cache[idx] = compute_a_composite(test);
            last_comp_val = test;
            ++idx;
        }
    }

    return cache[n - 1];
}
