/*
 * A332828  Expansion of (x + x^2 + x^6 - x^7)/(1 - x^2 + x^4 - x^6 + x^8) in
 *          powers of x.
 * https://oeis.org/A332828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in (x + x^2 + x^6 - x^7) / divided by
 *            (1 - x^2 + x^4 - x^6 + x^8). Recurrence (for n ≥ 8): a(n) =
 *            a(n-2) - a(n-4) + a(n-6) - a(n-8) Initial terms: a(0)=0, a(1)=1,
 *            a(2)=1, a(3)=1, a(4)=1, a(5)=0, a(6)=1, a(7)=-1
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate and fill an internal cache on first call. */
    static seq_t *cache = NULL;
    static size_t cap    = 0;   /* allocated capacity in sequence elements */
    static idx_t up      = 0;   /* highest index already computed */

    /* First call: set up a cache with at least 8 slots. */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            abort();              /* allocation failure */

        /* initial terms of the sequence */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)0;
        cache[6] = (seq_t)1;
        cache[7] = (seq_t)-1;

        up = 7;
    }

    /* Return a cached value if it already exists. */
    if (n <= up)
        return cache[(size_t)n];

    /* Extend cache if necessary. */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = cap;
        while ((size_t)(n + 1) > newcap)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms using the linear recurrence. */
    for (idx_t i = up + 1; i <= n; ++i) {
        /* Recurrence: a(i) = a(i-2) - a(i-4) + a(i-6) - a(i-8) */
        seq_t val = cache[(size_t)(i - 2)]
                   - cache[(size_t)(i - 4)]
                   + cache[(size_t)(i - 6)]
                   - cache[(size_t)(i - 8)];
        cache[(size_t)i] = val;
    }

    up = n;
    return cache[(size_t)n];
}
