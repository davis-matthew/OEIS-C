/*
 * A255374  Number of strings of n decimal digits that contain at least one
 *          string of exactly 4 consecutive "0" digits.
 * https://oeis.org/A255374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=a(1)=a(2)=a(3)=0, a(4)=1; for n>=5: a(n)=9 * (10^(n-5) -
 *            a(n-5) + sum_{i=4}^{n-1} a(i)) Sum_{i=4}^{n-1} a(i) =
 *            prefixSum[n-1]
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 22, u128 n <= 41,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow10.h"   /* assumes pow10(idx_t) returns seq_t */

static seq_t *A = NULL;    /* a[n] values   */
static seq_t *S = NULL;    /* prefix sums   */
static idx_t computed = 0; /* number of filled entries (computed indices 0..computed-1) */

/* compute a(n) with memoisation */
seq_t a(idx_t n)
{
    /* grow tables to length n+1 if needed */
    if (n + 1 > computed) {
        idx_t new_size = n + 1;
        A = (seq_t*)realloc(A, new_size * sizeof(seq_t));
        S = (seq_t*)realloc(S, new_size * sizeof(seq_t));
        for (idx_t i = computed; i <= n; ++i) {
            if (i < 4) {
                A[i] = (seq_t)0;
            } else if (i == 4) {
                A[i] = (seq_t)1;
            } else {
                /* 10^(i-5) */
                seq_t pow = pow10(i - 5);
                /* term = 10^(i-5) - A[i-5] + S[i-1] */
                seq_t term = pow;
                term -= A[i - 5];
                term += S[i - 1];
                term *= (seq_t)9;
                A[i] = term;
            }
            /* prefix sum S[i] = S[i-1] + A[i] (or just A[i] for i=0) */
            if (i == 0)
                S[i] = A[i];
            else
                S[i] = S[i - 1] + A[i];
        }
        computed = new_size;
    }
    return A[n];
}
