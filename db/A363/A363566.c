/*
 * A363566  G.f. satisfies A(x) = exp( Sum_{k>=1} (3 * (-1)^k + A(x^k)) *
 *          x^k/k ).
 * https://oeis.org/A363566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 a(n) = (1/n) * Σ_{k=1..n} ( 3*(-1)^k + Σ_{d|k} d *
 *            a(d-1) ) * a(n-k) for n>0
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 38, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) {          /* trivial case */
        seq_t r = 1;
        return r;
    }

    size_t size = (size_t)n + 1;   /* allocate arrays up to n */
    seq_t *A = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *S = (seq_t *)malloc(size * sizeof(seq_t));
    if (!A || !S) {        /* allocation failure */
        return 0;
    }
    memset(S, 0, size * sizeof(seq_t));

    /* a[0] = 1 */
    A[0] = 1;

    /* contributions with divisor d = 1: d*a[d-1] = 1*1 = 1, add to all multiples of 1 */
    for (idx_t m = 1; m <= n; ++m) {
        S[m] += (seq_t)1 * A[0];   /* A[0] == 1 */
    }

    /* compute terms a[1] .. a[n] */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t total = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = S[k];
            if (k & 1)           /* (-1)^k == -1 for odd k */
                term -= (seq_t)3;
            else
                term += (seq_t)3;
            total += term * A[i - k];
        }
        A[i] = total / (seq_t)i;

        /* update S for multiples of the new divisor d = i+1 */
        idx_t d = i + 1;
        for (idx_t m = d; m <= n; m += d) {
            S[m] += (seq_t)d * A[i];
        }
    }

    seq_t result = A[n];
    free(A);
    free(S);
    return result;
}
