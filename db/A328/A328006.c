/*
 * A328006  Expansion of e.g.f. 1 / (1 - Sum_{k>=1} binomial(2*k,k) * x^k / (k
 *          + 1)!).
 * https://oeis.org/A328006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * A000108(k) * a(n-k) with
 *            a(0) = 1 (Catalan numbers A000108 are computed via the
 *            recurrence C_0 = 1, C_n = C_{n-1} * (4n-2)/(n+1).)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *aCache = NULL;      /* values a(0..)       */
    static idx_t aCap  = 0;           /* current aCache size */
    static idx_t aComputed = 0;       /* index up to which aCache is filled  */

    static seq_t *catCache = NULL;    /* Catalan numbers C(0..) */
    static idx_t catCap  = 0;         /* current catCache size */
    static idx_t catComputed = 0;     /* index up to which catCache is filled  */

    /* Resize caches if necessary */
    if (aCap <= n) {
        size_t sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(aCache, sz * sizeof(seq_t));
        aCache = tmp;
        aCap = sz;
    }
    if (catCap <= n) {
        size_t sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(catCache, sz * sizeof(seq_t));
        catCache = tmp;
        catCap = sz;
    }

    /* Compute Catalan numbers up to n */
    if (catComputed <= n) {
        for (idx_t k = catComputed; k <= n; ++k) {
            if (k == 0) {
                catCache[0] = 1;
            } else {
                /* C_k = C_{k-1} * (4k-2)/(k+1) */
                seq_t num = catCache[k - 1] * (seq_t)(4 * k - 2);
                catCache[k] = num / (seq_t)(k + 1);
            }
        }
        catComputed = n + 1;
    }

    /* Base case */
    if (aComputed == 0) {
        aCache[0] = 1;
        aComputed = 1;
    }

    /* Compute a(i) for i from aComputed to n */
    if (aComputed <= n) {
        for (idx_t i = aComputed; i <= n; ++i) {
            seq_t sum = 0;
            seq_t binom = 1;          /* binom(i,0) */

            for (idx_t k = 1; k <= i; ++k) {
                /* binom(i,k) = binom(i,k-1) * (i-k+1) / k */
                seq_t mult = binom * (seq_t)(i - k + 1);
                binom = mult / (seq_t)k;

                seq_t term = binom * catCache[k];
                term = term * aCache[i - k];
                sum += term;
            }

            aCache[i] = sum;
        }
        aComputed = n + 1;
    }

    return aCache[n];
}
