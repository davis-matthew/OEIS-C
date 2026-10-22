/*
 * A352643  Expansion of e.g.f. exp(3 * x * cos(x)).
 * https://oeis.org/A352643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; a(n)=3*sum_{k=0..floor((n-1)/2)}
 *            (-1)^k*(2k+1)*binomial(n-1,2k)*a(n-2k-1)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 21, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* multiplicative binomial coefficient: C(a,b) */
static seq_t binom(idx_t a, idx_t b)
{
    if (b > a) return 0;          /* should not happen */
    if (b > a - b) b = a - b;
    seq_t r = 1;
    for (idx_t i = 1; i <= b; ++i) {
        r = r * (seq_t)(a - b + i); /* multiply first */
        r = r / (idx_t)i;           /* then divide */
    }
    return r;
}

seq_t a(idx_t n)
{
    /* cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;        /* allocated size */
    static idx_t   maxc   = 0;        /* highest computed index */

    /* allocate/reallocate if needed */
    if (n + 1 > cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise base case */
    if (maxc == 0 && cache != NULL) {
        cache[0] = 1;
        maxc = 0;
    }

    /* if already computed, return */
    if (n <= maxc)
        return cache[n];

    /* compute terms up to n */
    for (idx_t i = maxc + 1; i <= n; ++i) {
        idx_t maxk = (i - 1) / 2;
        int   sign = 1;           /* (-1)^k */
        seq_t sum = 0;

        for (idx_t k = 0; k <= maxk; ++k, sign = -sign) {
            seq_t bin = binom(i - 1, 2 * k);
            seq_t term = cache[i - 2 * k - 1];
            term = term * bin;
            term = term * (seq_t)(2 * k + 1);
            if (sign < 0)
                term = -term;
            sum += term;
        }

        cache[i] = 3 * sum;
    }

    maxc = n;
    return cache[n];
}
