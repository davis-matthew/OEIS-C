/*
 * A352302  Expansion of e.g.f. 1/(exp(x) - x^2).
 * https://oeis.org/A352302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n*(n-1)*a(n-2) - Sum_{k=1..n} binomial(n,k)*a(n-k) (n>1),
 *            with a(0)=1, a(1)=-1. The function memoizes computed values up
 *            to the maximum n reasonably requested (here 10000)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 11, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define MAX_TERMS 10001   /* supports n up to 10000 */

seq_t a(idx_t n)
{
    static seq_t cache[MAX_TERMS];
    static size_t next_to_compute = 0;   /* next index that needs computing */

    /* Limit visible range of n */
    if ((idx_t)next_to_compute > n) {
        return cache[(size_t)n];
    }
    if (n >= MAX_TERMS) {
        abort();   /* out of bounds: caller requests n higher than supported */
    }

    /* Initialise first two terms */
    if (next_to_compute == 0) {
        cache[0] = (seq_t)1;
        next_to_compute = 1;
    }
    if (next_to_compute == 1) {
        cache[1] = (seq_t)-1;
        next_to_compute = 2;
    }

    /* Compute terms from next_to_compute up to n */
    for (size_t i = next_to_compute; i <= (size_t)n; ++i) {
        /* For i >= 2, use the recurrence formula */
        seq_t term1 = (seq_t)i * (seq_t)(i - 1) * cache[i - 2];
        seq_t sum = 0;
        seq_t binom = (seq_t)i; /* binomial(i,1) */
        for (size_t k = 1; k <= i; ++k) {
            sum += binom * cache[i - k];
            if (k < i) {
                /* Update binomial(i,k+1) = binomial(i,k) * (i-k)/(k+1) */
                binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
            }
        }
        cache[i] = term1 - sum;
    }
    next_to_compute = n + 1;
    return cache[(size_t)n];
}
