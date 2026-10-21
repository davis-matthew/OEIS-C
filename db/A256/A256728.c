/*
 * A256728  Number of (n+2) X (1+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum zero and no antidiagonal sum three.
 * https://oeis.org/A256728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A256728: Number of (n+2)×(1+2) 0..1 arrays with no 3×3 subblock diagonal sum 0
 * and no antidiagonal sum three.
 *
 * Empirical recurrence (valid for n ≥ 5):
 *     a(n) = 7*a(n-1) - 56*a(n-3) + 64*a(n-4)
 *
 * Offset: 1
 *
 * The implementation uses memoised forward computation.
 * Arithmetic is performed with the native seq_t type, which works
 * correctly for both fixed-width unsigned integers (≤ 128 bits)
 * and arbitrary-precision integer classes.  No temporary
 * overflow outside of modular arithmetic for the fixed-width case. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache: indices start at 1 (cache[0] is unused). */
    static seq_t *cache = NULL;
    static idx_t  next  = 1;   /* next index that is not yet stored */
    static size_t cap   = 0;   /* allocated capacity */

    /* Guard against n == 0, which lies below the sequence offset. */
    if (n == 0)
        return 0;

    /* Initialise the cache with the first four known values if necessary. */
    if (cap < 5) {
        size_t new_cap = 5;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;  /* allocation failure: return 0 as a safe error value */
        cache = tmp;
        cap   = new_cap;
        /* 0 is unused at index 0 */
        cache[0] = 0;
        cache[1] = (seq_t)384;
        cache[2] = (seq_t)2304;
        cache[3] = (seq_t)13056;
        cache[4] = (seq_t)73984;
        next    = 5;
    }

    /* If the requested value is already inside the cache, return it. */
    if (n < next)
        return cache[n];

    /* Grow the cache if the requested index exceeds the currently allocated size. */
    if (n >= cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;   /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = next; i <= n; ++i) {
        /* a(i) = 7*a(i-1) + 64*a(i-4) - 56*a(i-3) */
        seq_t term7  = cache[i-1] * (seq_t)7;
        seq_t term64 = cache[i-4] * (seq_t)64;
        seq_t temp   = term7 + term64;          /* temporary sum */
        seq_t term56 = cache[i-3] * (seq_t)56;
        cache[i] = temp - term56;
    }

    next = n + 1;
    return cache[n];
}
