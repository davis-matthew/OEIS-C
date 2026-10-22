/*
 * A325303  a(1) = 1; a(n+1) = -Sum_{d|n} a(n/d) * a(d).
 * https://oeis.org/A325303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n+1) = -∑_{d|n} a(n/d) * a(d)
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 33, i64 n <= 65, i128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute OEIS sequence A325303 */
seq_t a(idx_t n)
{
    /* Memoised cache for previously computed terms */
    static seq_t *cache = NULL;   /* array of a(i) for i from 0 to cap-1 */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t last  = 0;      /* largest index computed so far */

    if (n < (idx_t)1) /* the sequence is defined only for n ≥ 1 */
        return (seq_t)0;

    /* Ensure the cache is large enough */
    if (cap <= (size_t)n) {
        size_t newcap = (cap == 0) ? 2 : (cap << 1);
        if (newcap <= (size_t)n) newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise base case if it has not yet been done */
    if (last < (size_t)1) {
        cache[0] = (seq_t)0;   /* unused, but convenient as filler */
        cache[1] = (seq_t)1;
        last    = 1;
    }

    /* Compute all terms up to the requested index */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        idx_t m = (idx_t)i - 1;   /* recurrence uses n‑1 */
        seq_t sum = (seq_t)0;

        /* enumerate divisors of m */
        for (idx_t d = 1; d <= m / d; ++d) {
            if (m % d == 0) {
                idx_t y = m / d;
                seq_t prod = cache[d] * cache[y];
                if (d == y)
                    sum += prod;          /* single divisor (square) */
                else
                    sum += prod * (seq_t)2; /* both d and y contribute */
            }
        }
        cache[i] = -sum;
    }

    last = (size_t)n;
    return cache[(size_t)n];
}
