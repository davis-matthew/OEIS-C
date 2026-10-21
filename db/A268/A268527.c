/*
 * A268527  a(n) = r*a(ceiling(n/2))+s*a(floor(n/2)) with a(1)=1 and
 *          (r,s)=(4,1).
 * https://oeis.org/A268527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(ceil(n/2)) + 1*a(floor(n/2)), a(1) = 1
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* memoized array of previously computed values */
static seq_t *cache = NULL;     /* dynamically resized cache */
static idx_t capacity = 0;      /* size of allocated cache (indices 0..capacity-1) */
static idx_t computed_up_to = 0; /* highest index already computed */

seq_t a(idx_t n)
{
    /* Ensure the cache can hold indices up to n */
    if (n >= capacity) {
        idx_t new_cap = n + 1; /* indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;    /* allocation failed – unlikely in this context */
        cache = tmp;
        capacity = new_cap;
    }

    /* Base initialization */
    if (computed_up_to == 0) {
        cache[1] = (seq_t)1;
        computed_up_to = 1;
    }

    /* If the requested value is already computed, return it */
    if (n <= computed_up_to)
        return cache[n];

    /* Compute all intermediate values up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        idx_t h = (i + 1) / 2; /* ceil(i/2) */
        idx_t l = i / 2;       /* floor(i/2) */
        cache[i] = (seq_t)4 * cache[h] + cache[l];
    }
    computed_up_to = n;
    return cache[n];
}
