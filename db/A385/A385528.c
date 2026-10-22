/*
 * A385528  E.g.f. A(x) satisfies A(x) = exp(x*A(-2*x)).
 * https://oeis.org/A385528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 9, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A385528
    a(0) = 1;
    a(n) = Σ_{k=0}^{n-1} (k+1) * (-2)^k * binomial(n-1,k) * a(k) * a(n-1-k)
    Formula and recurrence as stated above. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised cache of computed terms */
    static seq_t *cache = NULL;   /* array of terms */
    static idx_t cache_size = 0;  /* number of terms stored (cache[0..cache_size-1]) */

    /* Return cached value if already known */
    if (n < cache_size)
        return cache[n];

    /* Grow cache to hold values up to n */
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)                     /* allocation failure – safe default */
        return 0;
    cache = new_cache;

    /* Compute new terms from cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        seq_t value;
        if (i == 0) {
            value = 1;                       /* a(0)=1 */
        } else {
            seq_t sum = 0;
            seq_t binom = 1;                 /* C(i-1,0) */
            seq_t powNeg2 = 1;               /* (-2)^0 */
            idx_t N = i - 1;                 /* upper limit of k */

            for (idx_t k = 0; k <= N; ++k) {
                seq_t term = cache[k];
                term *= cache[N - k];
                term *= binom;
                term *= powNeg2;
                term *= (seq_t)(k + 1);
                sum += term;

                if (k < N) {                 /* prepare next k */
                    binom = binom * (seq_t)(N - k) / (seq_t)(k + 1);
                    powNeg2 *= -2;
                }
            }
            value = sum;
        }
        cache[i] = value;
    }

    cache_size = n + 1;  /* update cached size */
    return cache[n];
}
