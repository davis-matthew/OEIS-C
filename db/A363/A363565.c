/*
 * A363565  G.f. satisfies A(x) = exp( Sum_{k>=1} (2 * (-1)^k + A(x^k)) *
 *          x^k/k ).
 * https://oeis.org/A363565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 19, i16 n <= 36, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence from OEIS A363565):
   a(0) = 1;
   for n > 0
     a(n) = (1/n) * Σ_{k=1..n} ( 2*(-1)^k + Σ_{d|k} d * a(d-1) ) * a(n-k)   */

/* Seq_t and idx_t are defined by the build. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms and divisor sums */
    static seq_t *cache = NULL;   /* a[i] */
    static seq_t *S     = NULL;   /* S[i] = Σ_{d|i} d * a(d-1) */
    static size_t cap   = 0;      /* highest index already computed (cache size-1) */

    /* initialise cache for n = 0 */
    if (cache == NULL) {
        cache = (seq_t *)realloc(NULL, sizeof(seq_t));
        S     = (seq_t *)realloc(NULL, sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        S[0] = (seq_t)0;       /* unused, but set to 0 */
        cap = 0;
    }

    if (n <= cap)
        return cache[n];

    /* grow cache and S to new size n */
    size_t new_cap = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    seq_t *new_S     = (seq_t *)realloc(S,     (new_cap + 1) * sizeof(seq_t));
    if (new_cache == NULL || new_S == NULL)
        abort();                           /* allocation failure */

    cache = new_cache;
    S     = new_S;

    /* initialise new entries */
    for (idx_t i = (idx_t)(cap + 1); i <= n; ++i) {
        cache[i] = 0;
        S[i]     = 0;
    }

    /* compute terms a[i] for all i = cap+1 .. n */
    for (idx_t i = (idx_t)(cap + 1); i <= n; ++i) {
        /* compute S[i] = Σ_{d|i} d*a(d-1) */
        seq_t sumS = 0;
        for (idx_t d = 1; (idx_t)d * d <= i; ++d) {
            if (i % d == 0) {
                sumS += (seq_t)d * cache[d - 1];
                idx_t d2 = i / d;
                if (d2 != d)
                    sumS += (seq_t)d2 * cache[d2 - 1];
            }
        }
        S[i] = sumS;

        /* compute a[i] using the recurrence */
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t temp = S[k];
            temp += ((k & 1) == 0) ? 2 : -2;   /* 2*(-1)^k */
            sum += temp * cache[i - k];
        }
        cache[i] = sum / (seq_t)i;             /* division is exact */
    }

    cap = n;                                   /* update highest index */
    return cache[n];
}
