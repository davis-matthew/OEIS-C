/*
 * A269553  Expansion of (-5*x^2 + 138*x + 3)/(x^3 - 99*x^2 + 99*x - 1).
 * https://oeis.org/A269553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 4, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A269553 – expansion of (-5*x^2 + 138*x + 3)/(x^3 - 99*x^2 + 99*x - 1)
   Recurrence:  a(0) = -3, a(1) = -435, a(2) = -42763
                a(n) = a(n-3) - 99*a(n-2) + 99*a(n-1)   for n ≥ 3   */

/* Returns the nth term of the sequence.  The 0‑based index n is an
   unsigned 64‑bit integer.  The value is of type seq_t, which may be a
   signed plain integer of any bit‑length or an arbitrary‑precision
   integer class.  The implementation uses memoization so that
   calls in any order are O(n) overall.  Intermediate arithmetic is
   performed in seq_t to avoid overflow.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements presently stored */

    if (n < cache_size)
        return cache[n];

    /* Grow the cache to hold indices 0 … n */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        exit(1);          /* abort on allocation failure */
    cache = new_cache;

    /* Initialise the first term if this is the first allocation. */
    if (cache_size == 0)
        cache[0] = (seq_t)-3;

    /* Ensure the base values a(1) and a(2) are present when needed. */
    if (new_size > 1 && cache_size <= 1)    cache[1] = (seq_t)-435;
    if (new_size > 2 && cache_size <= 2)    cache[2] = (seq_t)-42763;

    /* Compute new terms via the recurrence. */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i <= 2)                 /* initial values already set */
            continue;
        cache[i] = cache[i-3]
                 - (seq_t)99 * cache[i-2]
                 + (seq_t)99 * cache[i-1];
    }

    cache_size = new_size;
    return cache[n];
}
