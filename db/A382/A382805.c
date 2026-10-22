/*
 * A382805  a(n) = Sum_{k=0..n} (-1)^(n-k) * (Stirling1(n,k) * k!)^2.
 * https://oeis.org/A382805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * (Stirling1(n,k) * k!)^2
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 12, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate arrays for current and previous rows of Stirling numbers */
    size_t len = (size_t)(n + 1);
    seq_t *prev = (seq_t*)calloc(len, sizeof(seq_t));
    seq_t *cur  = (seq_t*)malloc  (len * sizeof(seq_t));
    if (!prev || !cur) {
        free(prev);
        free(cur);
        return 0;
    }

    /* Factorial array */
    seq_t *fact = (seq_t*)calloc(len, sizeof(seq_t));
    if (!fact) {
        free(prev);
        free(cur);
        return 0;
    }
    fact[0] = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    /* Initialize Stirling numbers for n = 0 */
    prev[0] = 1; /* S1(0,0) = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        cur[0] = 0;           /* S1(i,0) = 0 for i > 0 */
        cur[i] = 1;           /* S1(i,i) = 1 */
        for (idx_t k = 1; k < i; ++k) {
            /* S1(i,k) = S1(i-1,k-1) + (i-1)*S1(i-1,k) */
            cur[k] = prev[k - 1] + (seq_t)(i - 1) * prev[k];
        }
        /* Prepare for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* Compute the sum */
    seq_t ans = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t s = prev[k];
        seq_t f = fact[k];
        seq_t prod = s * f;
        prod *= prod;                     /* (s*k!)^2 */
        if (((n - k) & 1) != 0) prod = -prod; /* multiply by (-1)^(n-k) */
        ans += prod;
    }

    free(prev);
    free(cur);
    free(fact);
    return ans;
}
