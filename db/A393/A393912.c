/*
 * A393912  a(n) = 1 + binomial(n+2,3) + Sum_{k=0..n-1} a(k) * a(n-1-k).
 * https://oeis.org/A393912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + binomial(n+2,3) + Sum_{k=0..n-1} a(k)*a(n-1-k)
 *            Initial values: a(0)=1, a(1)=3, a(2)=11, a(3)=42, a(4)=171
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Returns the nth term of OEIS sequence A393912.
   The function is thread‑safe only for single‑threaded use and
   memoises terms between calls so that arbitrary order of
   queries is supported. */
seq_t a(idx_t n)
{
    /* ----------------------------------------------------------
       static cache of computed values
       ---------------------------------------------------------- */
    static seq_t *cache   = NULL;    /* array of size 'cap' */
    static size_t cap     = 0;       /* current allocated size   */
    static idx_t max_done = (idx_t)-1; /* highest index computed */

    /* Guarantee space for 0..n */
    if (cache == NULL || cap < (size_t)(n + 1)) {
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp    = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) abort();          /* out of memory */
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise a(0) on first entry */
    if (max_done == (idx_t)-1) {
        cache[0] = (seq_t)1;
        max_done = 0;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_done + 1; i <= n; ++i) {
        /* binomial(i+2,3) = (i+2)(i+1)i / 6  */
        seq_t binom = ((seq_t)(i + 2) * (seq_t)(i + 1) * (seq_t)i) / 6;

        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            sum += cache[k] * cache[i - 1 - k];
        }

        cache[i] = 1 + binom + sum;
    }

    /* Update highest computed index */
    if (n > max_done) max_done = n;

    return cache[n];
}
