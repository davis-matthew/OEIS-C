/*
 * A349513  a(n) = n! * Sum_{k=0..n} (2*k)! / (k!)^3.
 * https://oeis.org/A349513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*Sum_{k=0..n} (2k)!/(k!)^3 (recurrence a(n) = n*a(n‑1)
 *            + C(2n,n))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute A349513: a(n) = n!*Sum_{k=0..n} (2k)!/(k!)^3   */
seq_t a(idx_t n)
{
    /* static cache of computed values and associated state */
    static seq_t *cache   = NULL;     /* array of computed a(i) */
    static size_t  cap    = 0;        /* allocated capacity of cache   */
    static idx_t   computed = 0;      /* highest n for which a(n) known */
    static seq_t   central  = 1;      /* C(2*m,m) for current m        */

    /* Ensure cache is initialised */
    if (cache == NULL) {
        /* start with a single element a(0)=1 */
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return 0;            /* allocation failure */
        cache[0] = (seq_t)1;
        cap    = 1;
        computed = 0;
        central  = (seq_t)1;                    /* C(0,0) */
    }

    /* Grow the cache if necessary */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;             /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute values up to n if not already done */
    for (idx_t m = computed + 1; m <= n; ++m) {
        /* Update the central binomial coefficient C(2*m,m) */
        central = (central * (seq_t)(4*m - 2)) / (seq_t)m;
        /* Recurrence: a(m) = m*a(m-1) + C(2*m,m) */
        cache[m] = cache[m-1] * (seq_t)m + central;
    }

    computed = n;

    return cache[n];
}
