/*
 * A358118  a(n) = Sum_{j=0..n} (-1)^j*binomial(2*n - j, j)*c(n - j + 1)^2,
 *          where c(n) is the n-th Catalan number.
 * https://oeis.org/A358118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} (-1)^j * binomial(2*n - j, j) * catalan(n -
 *            j + 1)^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Precompute Catalan numbers C[0 .. n+1] */
    seq_t *C = (seq_t *)malloc((size_t)(n + 2) * sizeof(seq_t));
    if (!C)
        return (seq_t)0;               /* unlikely, but avoid crash */

    C[0] = (seq_t)1;                   /* Catalan(0) */
    /* Catalan recurrence: C_{k+1} = C_k * 2*(2*k+1) / (k+2) */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t mult = (seq_t)2 * (seq_t)(2 * k + 1);
        C[k + 1] = (C[k] * mult) / (k + 2);
    }

    seq_t sum_pos = 0, sum_neg = 0;
    seq_t f = (seq_t)1;                 /* f = binomial(2*n - j, j), j=0 */

    for (idx_t j = 0; j <= n; ++j) {
        seq_t c = C[n - j + 1];
        seq_t term = f * c;
        term = term * c;                /* (f * c^2) */

        if ((j & 1U) == 0)
            sum_pos += term;
        else
            sum_neg += term;

        if (j == n)
            break;

        /* Update f to f_{j+1} */
        seq_t a = (seq_t)2 * n - (seq_t)2 * j;
        seq_t b = a - (seq_t)1;
        seq_t denom1 = (seq_t)(j + 1);
        seq_t denom2 = (seq_t)2 * n - (seq_t)j;

        f = (f * a) / denom1;          /* exact division */
        f = (f * b) / denom2;          /* exact division */
    }

    free(C);
    return sum_pos - sum_neg;
}
