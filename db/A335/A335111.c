/*
 * A335111  a(n) = n! * Sum_{k=0..n-1} (-2)^k / k!.
 * https://oeis.org/A335111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..n-1} (-2)^k / k!
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 23,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(n+1) = (n+1) * ( a(n) + (-2)^n ) */

#include <stddef.h>
#include <stdbool.h>
/* utils function guaranteed to exist */
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    /* static memoization state */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t next_computed = 0;   /* maximum index already computed */
    static seq_t pow_minus_two = 1;    /* (-2)^current_index */

    /* initialise cache once */
    if (cache == NULL) {
        ensure_capacity(1, &cache_cap, (void **)&cache, sizeof(*cache));
        cache[0] = 0;               /* a(0) = 0 */
        pow_minus_two = 1;          /* (-2)^0 = 1 */
        next_computed  = 0;
    }

    /* return already computed value if possible */
    if (n <= next_computed) {
        return cache[n];
    }

    /* grow cache to fit up to n */
    ensure_capacity((size_t)n + 1, &cache_cap, (void **)&cache, sizeof(*cache));

    /* compute values from next_computed+1 up to n using recurrence */
    for (idx_t i = next_computed; i < n; ++i) {
        seq_t term = cache[i] + pow_minus_two;           /* a(i) + (-2)^i */
        cache[i + 1] = ((seq_t)(i + 1)) * term;          /* (i+1)*(a(i)+(-2)^i) */
        pow_minus_two = pow_minus_two * (seq_t)(-2);      /* update (-2)^(i+1) */
    }

    next_computed = n;
    return cache[n];
}
