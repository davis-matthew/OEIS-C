/*
 * A291916  Number of (not necessarily maximal) cliques in the n-Fibonacci
 *          cube graph.
 * https://oeis.org/A291916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A291916: Number of (not necessarily maximal) cliques in the n‑Fibonacci cube graph.
   Recurrence: a(n) = 3*a(n-1) - a(n-2) - 3*a(n-3) + a(n-4) + a(n-5),
   with initial values: a(1)=4, a(2)=6, a(3)=11, a(4)=19, a(5)=34. */

#include <stdlib.h>   /* for malloc, realloc, abort */

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n<1, but return 0 */

    /* static cache of computed sequence values (indices 1..cache_len) */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;   /* allocated size (elements 0..cache_cap-1) */
    static size_t cache_len = 0;   /* highest index computed so far */

    /* Grow cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;          /* include index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                      /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        if (i <= 5) {                           /* initial values */
            switch (i) {
                case 1: cache[i] = (seq_t)4;  break;
                case 2: cache[i] = (seq_t)6;  break;
                case 3: cache[i] = (seq_t)11; break;
                case 4: cache[i] = (seq_t)19; break;
                case 5: cache[i] = (seq_t)34; break;
            }
        } else {                                 /* recurrence for i ≥ 6 */
            cache[i] = cache[i-1] * 3
                       - cache[i-2]
                       - cache[i-3] * 3
                       + cache[i-4]
                       + cache[i-5];
        }
    }
    cache_len = (size_t)n;

    return cache[(size_t)n];
}
