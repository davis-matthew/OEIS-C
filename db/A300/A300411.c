/*
 * A300411  a(0) = a(1) = 1; a(n) = [x^n] Product_{k=1..n-1} 1/(1 -
 *          a(k)*x^a(k)).
 * https://oeis.org/A300411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 27, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A300411: a(0)=a(1)=1; a(n) = [x^n] ∏_{k=1}^{n-1} 1/(1 - a(k)·x^{a(k)}) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 1;

    idx_t size = n + 1;
    seq_t *coeff = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *a_vals = (seq_t *)calloc(size, sizeof(seq_t));
    if (!coeff || !a_vals)
    {
        /* allocation failed – return 0 as a safe sentinel */
        if (coeff) free(coeff);
        if (a_vals) free(a_vals);
        return 0;
    }

    /* initial conditions */
    coeff[0] = 1;
    a_vals[0] = 1;
    a_vals[1] = 1;              /* a(1) = 1 */

    for (idx_t k = 1; k <= n - 1; ++k)
    {
        seq_t ak = a_vals[k];
        /* if part size exceeds n, this factor never contributes to [x^≤n] */
        if (ak > (seq_t)n)
        {
            a_vals[k + 1] = coeff[k + 1];
            continue;
        }
        idx_t step = (idx_t)ak;          /* ak ≤ n, safe cast */
        for (idx_t e = step; e <= n; ++e)
        {
            coeff[e] += coeff[e - step] * ak;
        }
        a_vals[k + 1] = coeff[k + 1];
    }

    seq_t result = coeff[n];
    free(coeff);
    free(a_vals);
    return result;
}
