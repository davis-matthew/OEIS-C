/*
 * A247527  Number of length n+3 0..2 arrays with some disjoint pairs in every
 *          consecutive four terms having the same sum.
 * https://oeis.org/A247527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-4) - 4*a(n-5) (empirical recurrence)
 *            initial conditions: a(1)=33, a(2)=45, a(3)=61, a(4)=81, a(5)=105
 *            The function memoises results so that subsequent calls are O(1).
 *            It works for any seq_t/idx_t types supplied by the build
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 55, u64 n <= 119, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;        /* dynamic array of cached terms   */
static size_t cache_size = 0;      /* allocated number of slots       */
static idx_t   max_computed = 0;   /* largest n that has been computed */

/* Return the nth term of the sequence (n >= 1).  Undefined for n < 1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* n=0 or negative input is undefined */

    /* Ensure the cache is large enough for indices 0..n. */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();         /* allocation failed */
        /* Zero‑initialize the new elements (safe for integer and
           arbitrary‑precision types that support assignment from zero). */
        for (size_t i = cache_size; i < new_size; ++i)
            tmp[i] = (seq_t)0;
        cache       = tmp;
        cache_size  = new_size;
    }

    /* Initialize the first five terms (if not already).  They are
       overwritten only when needed, which is harmless. */
    if (n >= 1) cache[1] = (seq_t)33;
    if (n >= 2) cache[2] = (seq_t)45;
    if (n >= 3) cache[3] = (seq_t)61;
    if (n >= 4) cache[4] = (seq_t)81;
    if (n >= 5) cache[5] = (seq_t)105;

    if (max_computed < 5 && n >= 5) max_computed = 5;   /* base terms set */

    /* Compute any missing terms up to n using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 5) continue;            /* base values already set */
        /* a(i) = a(i-1) + 4*(a(i-4)-a(i-5))  to limit intermediate size */
        seq_t diff = cache[i - 4] - cache[i - 5];
        seq_t add  = diff * (seq_t)4;
        cache[i] = cache[i - 1] + add;
    }
    max_computed = n > max_computed ? n : max_computed;
    return cache[n];
}
