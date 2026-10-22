/*
 * A335980  Expansion of e.g.f. exp(2 * (1 - exp(-x)) + x).
 * https://oeis.org/A335980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 17, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A335980: a(n) = exp(2) * (-1)^n * Sum_{k>=0} (-2)^k * (k-1)^n / k!
   Recurrence: a(0)=1; a(n)=a(n-1)+2*Sum_{k=0}^{n-1} (-1)^{n-k-1}*C(n-1,k)*a(k)
   Useful for all n≥0.  Cache results for arbitrary order of calls.
*/

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* allocated capacity */
    static size_t   last  = 0;   /* largest index computed so far */

    /* initialise cache for n=0 */
    if (cache == NULL) {
        cap  = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;
        last  = 0;
    }

    /* if requested n is beyond current cache, extend and compute */
    if ((size_t)n > last) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;

        for (size_t i = last + 1; i <= (size_t)n; ++i) {
            seq_t sum   = 0;
            seq_t binom = 1;             /* C(n-1,0) */
            for (size_t k = 0; k < i; ++k) {
                seq_t prod = cache[k] * binom;
                if (((i - 1 - k) & 1) == 0)
                    sum += prod;
                else
                    sum -= prod;

                if (k + 1 < i) {
                    binom = binom * (seq_t)(i - 1 - k);
                    binom = binom / (seq_t)(k + 1);   /* exact division */
                }
            }
            cache[i] = cache[i - 1] + (sum + sum);   /* a(i) = a(i‑1) + 2*sum */
        }

        last = n;
    }

    return cache[n];
}
