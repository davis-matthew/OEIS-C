/*
 * A279271  Exponential transform of the Pell numbers.
 * https://oeis.org/A279271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A279271
 * Formula:  A(x) = exp( exp(x) * sinh(sqrt(2)*x) / sqrt(2) )
 *
 * The sequence is the exponential transform of the Pell numbers
 * P(0)=0, P(1)=1, P(n)=2*P(n-1)+P(n-2).
 *
 * Recurrence derived from the derivative of the EGF:
 *   a(0) = 1
 *   a(n+1) = Σ_{k=0}^{n} binom(n, k) * a(k) * P(n-k+1)
 *
 * This implementation uses only integer arithmetic on the type
 * seq_t; no floating‑point operations are required.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Quick return for n == 0 */
    if (n == 0)
        return 1;

    /* Allocate arrays for pell numbers and sequence values */
    idx_t pell_size = n + 2;               /* need P[0..n+1]          */
    idx_t a_size   = n + 1;               /* need a[0..n]           */
    seq_t *pell = (seq_t *)malloc(pell_size * sizeof(seq_t));
    seq_t *a_arr = (seq_t *)malloc(a_size  * sizeof(seq_t));
    if (!pell || !a_arr) {
        /* Allocation failure – return 0 as a safe fallback */
        if (pell) free(pell);
        if (a_arr) free(a_arr);
        return 0;
    }

    /* Pell numbers: P(0)=0, P(1)=1, P(k)=2*P(k-1)+P(k-2) */
    pell[0] = 0;
    pell[1] = 1;
    for (idx_t i = 2; i <= n + 1; ++i)
        pell[i] = pell[i - 1] * ((seq_t)2) + pell[i - 2];

    /* Sequence initialization */
    a_arr[0] = 1;   /* a(0) = 1 */

    /* Compute a(n) for n = 1 .. N using the recurrence */
    for (idx_t cur = 0; cur < n; ++cur) {
        seq_t sum  = 0;
        seq_t binom = 1;        /* binom(cur, 0) */

        for (idx_t k = 0; k <= cur; ++k) {
            /* term = binom * a(k) * P(cur - k + 1) */
            seq_t term = binom * a_arr[k];
            term = term * pell[cur - k + 1];
            sum += term;

            /* Update binomial coefficient to binom(cur, k+1) */
            if (k < cur) {
                seq_t numer = binom * ((seq_t)(cur - k));
                binom = numer / ((seq_t)(k + 1));
            }
        }
        a_arr[cur + 1] = sum;
    }

    seq_t result = a_arr[n];
    free(pell);
    free(a_arr);
    return result;
}
