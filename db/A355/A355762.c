/*
 * A355762  E.g.f. satisfies log(A(x)) = (exp(x*A(x)) - 1) * A(x)^2.
 * https://oeis.org/A355762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} (n + 2*k + 1)^(k-1) * Stirling2(n,k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* handle the special case n == 0 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate array for current row of Stirling numbers S(k). */
    size_t size = (size_t)n + 1;              /* indices 0..n */
    seq_t *S = (seq_t *)calloc(size, sizeof(seq_t));
    if (!S) {
        return (seq_t)0; /* allocation failed; caller will treat as zero */
    }

    S[0] = 1; /* S(0,0) = 1 */

    /* Build rows 1..n of Stirling numbers S(n,k) using the recurrence
       S(n,k) = S(n-1,k-1) + k * S(n-1,k)
       The array S is updated in place from high k to low k. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t k = i; k >= 1; --k) {
            /* S(k) = S(k-1) + k * S(k)   (S(k) on RHS is old value) */
            S[k] = S[k - 1] + S[k] * (seq_t)k;
        }
        S[0] = 0; /* S(n,0) = 0 for n > 0 */
    }

    /* Compute the desired sum Σ_{k=1..n} (n+2*k+1)^(k-1) * S(k) */
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t base    = (seq_t)n + (seq_t)(2 * k) + (seq_t)1;
        seq_t power   = big_pow(base, (seq_t)(k - 1));
        result += power * S[k];
    }

    free(S);
    return result;
}
