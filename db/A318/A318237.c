/*
 * A318237  E.g.f. satisfies y'' + y' + x^3*y = 0 with y(0)=1, y'(0)=0.
 * https://oeis.org/A318237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   Recurrence (n+3)(n+2)(n+1)*a(n) + a(n+4) + a(n+5) = 0 Initial
 *            values: a(0)=1, a(1)=a(2)=a(3)=a(4)=0
 * Range:     i8 n <= 9, i16 n <= 14, i32 n <= 19, i64 n <= 30, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t last = (idx_t)-1;
    static size_t cap = 0;

    /* initialise cache with first five terms */
    if (cache == NULL) {
        cap = 5;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* memory failure */
        cache[0] = (seq_t)1;
        cache[1] = cache[2] = cache[3] = cache[4] = (seq_t)0;
        last = 4;
    }

    /* grow cache if required */
    if (n >= cap) {
        size_t new_cap = cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* memory failure */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n > last) {
        for (idx_t k = last + 1; k <= n; ++k) {
            if (k < 5) continue;           /* already initialised */
            idx_t i = k - 5;
            seq_t prod = ((seq_t)(i + 3)) * ((seq_t)(i + 2)) * ((seq_t)(i + 1));
            seq_t term = prod * cache[i];
            cache[k] = -term - cache[i + 4];
        }
        last = n;
    }

    return cache[n];
}
