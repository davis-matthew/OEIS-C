/*
 * A392898  a(n) = a(n-1) + a(n-2) + n! with a(0)=0, a(1)=1.
 * https://oeis.org/A392898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; a(1) = 1; a(n) = a(n-1) + a(n-2) + n! for n ≥ 2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A392898.
   Supports arbitrary built‑in integer types seq_t and idx_t.
   Uses a static cache so that calls in any order remain efficient.  */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache variables */
    static seq_t *cache = NULL;     /* cached sequence values */
    static idx_t cache_len = 0;     /* number of items stored in cache */
    static seq_t fact = 1;          /* factorial of fact_n */
    static idx_t fact_n = 1;        /* so fact = fact_n!  (fact_n starts at 1 for 1!) */

    /* Initialise cache for n = 0, 1 */
    if (!cache) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;
        cache[1] = 1;
        cache_len = 2;
    }

    /* If we already have the requested term, return it immediately */
    if (n < cache_len)
        return cache[n];

    /* We need to extend the cache up to n */
    size_t old_len = cache_len;               /* length before extending   */
    if ((idx_t)old_len <= n) {                /* more space needed?        */
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = n + 1;
    }

    /* Compute missing terms from old_len up to n */
    for (idx_t i = old_len; i <= n; ++i) {
        /* factorial: multiply incrementally from fact_n+1 up to i */
        for (idx_t k = fact_n + 1; k <= i; ++k)
            fact *= (seq_t)k;
        fact_n = i;

        /* recurrence: a(i) = a(i-1) + a(i-2) + i!   (i ≥ 2) */
        cache[i] = cache[i - 1] + cache[i - 2] + fact;
    }

    return cache[n];
}
