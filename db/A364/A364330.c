/*
 * A364330  G.f. satisfies A(x) = (1 + x^4) * (1 + x*A(x)^2).
 * https://oeis.org/A364330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   OEIS A364330 G.f.: A(x) = (1 + x^4) * (1 + x*A(x)^2) Sparked by
 *            recurrence: (n+1)*a(n) + 2*(-2n+1)*a(n-1) + (n+1)*a(n-4) +
 *            6*(-2n+9)*a(n-5) + 6*(-2n+17)*a(n-9) + 2*(-2n+25)*a(n-13) = 0
 *            For n >= 13, a(n) = [(4n-2)a(n-1)+(12n-54)a(n-5)
 *            +(12n-102)a(n-9)+(4n-50)a(n-13)-(n+1)a(n-4)]/(n+1); Base
 *            values...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"   /* provides: seq_t binomial(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of values computed so far */

    if (n < cache_len)           /* already computed */
        return cache[n];

    /* grow cache to hold n+1 entries */
    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        abort();
    cache = new_cache;

    /* compute missing values */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 13) {  /* use combinatorial definition for base cases */
            seq_t sum = 0;
            idx_t maxk = i / 4;
            for (idx_t k = 0; k <= maxk; ++k) {
                idx_t t = 2 * i - 8 * k + 1;
                seq_t term = binomial(t, k) * binomial(t, i - 4 * k);
                term = term / (seq_t)t;   /* exact division */
                sum += term;
            }
            cache[i] = sum;
        } else {       /* use recurrence for i >= 13 */
            /* compute numerator = (4i-2)a(i-1)+(12i-54)a(i-5)
               +(12i-102)a(i-9)+(4i-50)a(i-13)-(i+1)a(i-4) */
            seq_t term1  = (seq_t)(4 * i - 2)  * cache[i - 1];
            seq_t term5  = (seq_t)(12 * i - 54) * cache[i - 5];
            seq_t term9  = (seq_t)(12 * i - 102) * cache[i - 9];
            seq_t term13 = (seq_t)(4 * i - 50)  * cache[i - 13];
            seq_t term4  = (seq_t)(i + 1)       * cache[i - 4];

            seq_t numerator = term1 + term5 + term9 + term13;
            numerator -= term4;   /* guaranteed non‑negative for i>=13 */

            cache[i] = numerator / (seq_t)(i + 1);
        }
    }

    cache_len = n + 1;
    return cache[n];
}
