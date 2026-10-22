/*
 * A332776  a(n) = 1 + Sum_{k=1..n-1} binomial(n-1,k) * a(k) * a(n-k-1).
 * https://oeis.org/A332776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=1}^{n-1} C(n-1,k)·a(k)·a(n-k-1) Initial value:
 *            a(0) = 1 (a(1) follows from the recurrence)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation (0 … max) */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static idx_t   max    = (idx_t)-1;   /* uninitialised */

    /* ensure cache contains a(0) = 1 initially */
    if (max == (idx_t)-1) {
        /* allocate space for a(0) */
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;
        cap    = 1;
        max    = 0;
    }

    /* grow cache if needed */
    if ((idx_t)n >= (idx_t)cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute new values up to n (in any order of calls) */
    for (idx_t m = max + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;            /* C(m-1,0) */

        for (idx_t k = 1; k <= m - 1; ++k) {
            /* update bin to C(m-1,k) */
            bin = bin * (seq_t)(m - k) / (seq_t)k;

            /* multiply a(k) * a(m-k-1) */
            seq_t prod = cache[k] * cache[m - k - 1];

            /* add contribution to sum */
            sum += bin * prod;
        }

        cache[m] = sum + (seq_t)1;   /* a(m) = 1 + sum */
    }
    max = n;

    return cache[n];
}
