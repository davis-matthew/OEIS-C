/*
 * A267633  Expansion of (1 - 4t)/(1 - x + t x^2): a Fibonacci-type sequence
 *          of polynomials.
 * https://oeis.org/A267633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabf
 * Formula:   a(n) = coefficient of t^k in the polynomial P(m,t) where the
 *            polynomials are enumerated row‑wise. Here P(0)=1-4t, P(1)=1-4t,
 *            and for m≥2, P(m)=P(m-1)-t*P(m-2). The row for index m has
 *            length floor(m/2)+2. The function computes the k‑th coefficient
 *            of the polynomial whose row position m is the one...
 * Range:     i8 n <= 42, i16 n <= 53, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* locate the row m and column k that correspond to index n */
    idx_t m   = 0;
    idx_t acc = 0;
    while (1) {
        idx_t len = (m >> 1) + 2;          /* row length = ⌊m/2⌋+2 */
        if (n < acc + len)
            break;
        acc += len;
        ++m;
    }
    idx_t k = n - acc;                    /* column within row m */

    /* rows 0 and 1 are identical: [1, -4] */
    if (m == 0 || m == 1) {
        return (k == 0) ? (seq_t)1 : (seq_t)(-4);
    }

    /* maximum row length needed for n≈10 000 is far below 200;
       choose a generous constant to eliminate dynamic allocation. */
    const idx_t MAXLEN = 2048;
    seq_t prev2[MAXLEN];
    seq_t prev1[MAXLEN];
    seq_t curr [MAXLEN];

    /* initialise rows 0 and 1 */
    prev2[0] = prev1[0] = (seq_t)1;
    prev2[1] = prev1[1] = (seq_t)(-4);
    idx_t len_prev2 = 2;
    idx_t len_prev1 = 2;

    /* iterate until the required row m is reached */
    for (idx_t i = 2; i <= m; ++i) {
        idx_t len_curr = (i >> 1) + 2;

        /* compute the coefficients of P(i) = P(i-1) – t·P(i-2) */
        for (idx_t j = 0; j < len_curr; ++j) {
            seq_t val = 0;
            if (j < len_prev1)          /* coefficient from P(i-1) */
                val += prev1[j];
            if (j > 0 && (j - 1) < len_prev2)  /* subtract shifted P(i-2) */
                val -= prev2[j - 1];
            curr[j] = val;
        }

        /* shift rows for the next iteration */
        for (idx_t j = 0; j < len_prev1; ++j)
            prev2[j] = prev1[j];
        for (idx_t j = 0; j < len_curr; ++j)
            prev1[j] = curr[j];

        len_prev2 = len_prev1;
        len_prev1 = len_curr;
    }

    return prev1[k];
}
