/*
 * A384029  a(n) = [x^n] Product_{k=0..n-1} (1 + k*x)^4.
 * https://oeis.org/A384029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in ∏_{k=0}^{n-1} (1 + k*x)^4
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate array of coefficients 0..n. */
    seq_t *res = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!res)
        return (seq_t)0;   /* allocation failure */

    for (idx_t i = 0; i <= n; ++i)
        res[i] = (seq_t)0;
    res[0] = (seq_t)1;

    for (idx_t k = 0; k < n; ++k) {
        seq_t kseq = (seq_t)k;
        seq_t k2   = kseq * kseq;
        seq_t k3   = k2 * kseq;
        seq_t k4   = k3 * kseq;

        /* Update coefficients in place, descending to keep old values. */
        for (idx_t d = n; ; --d) {
            seq_t t = res[d];
            if (d >= 1)
                t += (seq_t)4 * kseq * res[d - 1];
            if (d >= 2)
                t += (seq_t)6 * k2 * res[d - 2];
            if (d >= 3)
                t += (seq_t)4 * k3 * res[d - 3];
            if (d >= 4)
                t += k4 * res[d - 4];
            res[d] = t;
            if (d == 0)
                break;
        }
    }

    seq_t result = res[n];
    free(res);
    return result;
}
