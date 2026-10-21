/*
 * A246509  G.f.: Sum_{n>=0} x^n / (1-3*x)^(2*n+1) * [Sum_{k=0..n} C(n,k)^2 *
 *          3^k * x^k] * [Sum_{k=0..n} C(n,k)^2 * 4^k * x^k].
 * https://oeis.org/A246509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} 4^k * Sum_{j=0..n-2*k} C(n-k,
 *            k+j)^2 * C(k+j, j)^2 * 3^j
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow4 = 1;                     /* 4^k, starts with k=0 */

    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - k;

        /* Compute C(m, k) multiplicatively */
        seq_t a_val = 1;
        for (idx_t i = 1; i <= k; ++i) {
            a_val = a_val * (m - k + i) / i;   /* C(m,k) */
        }

        seq_t b_val = 1;            /* C(k,0) = 1 */
        seq_t pow3  = 1;            /* 3^j, starts with j=0 */

        idx_t maxj = n - 2 * k;
        for (idx_t j = 0; j <= maxj; ++j) {
            /* term = 4^k * (C(m, k+j)^2) * (C(k+j, j)^2) * 3^j */
            seq_t term = a_val * a_val;        /* C(m, k+j)^2 */
            term = term * (b_val * b_val);    /* *C(k+j, j)^2 */
            term = term * pow3;               /* *3^j */
            term = term * pow4;               /* *4^k */
            result += term;

            /* Update values for next j, if any */
            if (j < maxj) {
                seq_t numer_a = m - (k + j);
                seq_t denom_a = k + j + 1;
                a_val = a_val * numer_a / denom_a;    /* next C(m, k+j+1) */

                seq_t numer_b = k + j + 1;
                seq_t denom_b = j + 1;
                b_val = b_val * numer_b / denom_b;    /* next C(k+j+1, j+1) */

                pow3 = pow3 * 3;                       /* next 3^(j+1) */
            }
        }

        pow4 = pow4 * 4; /* prepare 4^(k+1) for next outer iteration */
    }

    return result;
}
