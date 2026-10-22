/*
 * A355164  a(n) = exp(-1/3) * Sum_{k>=0} (3*k + 2)^n / (3^k * k!).
 * https://oeis.org/A355164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1: a(n)=2*a(n-1) * + Σ_{k=1..n} binomial(n-1,k-1)
 *            * 3^(k-1) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t  capacity = 0;   /* allocated size */
    static idx_t  computed = 0;   /* number of terms already computed */

    /* grow cache if needed */
    if (capacity < n + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;     /* allocation failed – unlikely */
        cache = tmp;
        capacity = n + 1;
    }

    /* initialise first term */
    if (computed == 0) {
        cache[0] = 1;
        computed = 1;
    }

    /* compute any missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0) continue;           /* already set */

        seq_t sum   = 0;
        seq_t binom = 1;                /* binomial(i-1,0) */
        seq_t pow3  = 1;                /* 3^(k-1), start with k=1 */

        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = cache[i - k];
            term = term * binom;
            term = term * pow3;
            sum  = sum + term;

            if (k < i) {
                /* update binomial for next k:
                 * binom := binom * (i-k) / k
                 */
                seq_t num = (seq_t)(i - k);
                seq_t den = (seq_t)k;
                binom = (binom * num) / den;
                pow3 = pow3 * 3;
            }
        }

        cache[i] = cache[i - 1] * 2 + sum;
    }

    computed = n + 1;
    return cache[n];
}
