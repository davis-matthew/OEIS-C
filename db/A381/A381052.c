/*
 * A381052  Expansion of e.g.f. log(1 - x)^2 * exp(3*x) / 2.
 * https://oeis.org/A381052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n} 3^(n-k) * binomial(n,k) * |Stirling1(k,2)|
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    /* Allocate array for |Stirling1(k,2)| values (0..n). */
    size_t len = (size_t)(n + 1);
    seq_t *t = (seq_t *)malloc(len * sizeof(seq_t));
    if (!t) return (seq_t)0; /* out of memory: return 0 */

    /* Initialise base cases */
    t[0] = (seq_t)0;
    t[1] = (seq_t)0;
    t[2] = (seq_t)1;

    /* Compute Stirling numbers incrementally: t[k] = |S1(k,2)| */
    seq_t factPrev = (seq_t)1;   /* (k-2)! for current k (start k=3) */
    seq_t tPrev   = t[2];        /* t[2] */

    for (idx_t k = 3; k <= n; ++k) {
        seq_t tCur = (seq_t)(k - 1) * tPrev + factPrev;  /* t[k] */
        t[k] = tCur;
        tPrev = tCur;
        factPrev *= (seq_t)(k - 1);   /* factPrev becomes (k-1)! for next round */
    }

    /* Sum over k = n down to 2, updating binomial and power of 3. */
    seq_t sum   = (seq_t)0;
    seq_t binom = (seq_t)1;   /* binomial(n,n) = 1 */
    seq_t power = (seq_t)1;   /* 3^(n-n) = 1 */

    for (idx_t k = n; k >= 2; --k) {
        seq_t term = t[k] * binom * power;
        sum += term;

        if (k > 2) {
            /* Update binomial coefficient for next k-1:
               binom(n,k-1) = binom(n,k) * k / (n - k + 1) */
            seq_t num = (seq_t)k;
            seq_t den = (seq_t)(n - k + 1);
            binom = binom * num / den;

            /* Update power of 3: next term needs 3^(n-(k-1)) = 3 * 3^(n-k) */
            power *= (seq_t)3;
        }
    }

    free(t);
    return sum;
}
