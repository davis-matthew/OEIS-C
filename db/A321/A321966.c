/*
 * A321966  Triangle read by rows, coefficients of a family of orthogonal
 *          polynomials, T(n, k) for 0 <= k <= n.
 * https://oeis.org/A321966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) computed from the recurrence of orthogonal polynomials
 *            p(n,x) with coef T(n,k) = coefficient of x^k. The triangle read
 *            by rows gives the seq
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Find row r and column k: row r has r+1 entries,
       cumulative entries up to row r-1 is r(r+1)/2. */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum + (r + 1) <= n) {
        sum += r + 1;
        r += 1;
    }
    idx_t k = n - sum;           /* 0 <= k <= r */

    /* Base cases: rows 0, 1, 2 */

    if (r == 0) {
        (void)k; /* silence unused warning */
        return (seq_t)1;
    }
    if (r == 1) {
        return (k == 0) ? (seq_t)1 : (seq_t)1;
    }
    if (r == 2) {
        if (k == 0) return (seq_t)2;
        if (k == 1) return (seq_t)5;
        return (seq_t)1;
    }

    /* Allocate workspace: four rows of length r+1. */
    size_t len = (size_t)(r + 1);
    seq_t *p0 = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *p1 = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *p2 = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *p3 = (seq_t *)malloc(len * sizeof(seq_t));
    if (!p0 || !p1 || !p2 || !p3) return (seq_t)0;  /* safety */

    /* Initialize all entries to 0 */
    for (size_t i = 0; i < len; ++i) {
        p0[i] = p1[i] = p2[i] = p3[i] = (seq_t)0;
    }

    /* Set known rows 0, 1, 2 */
    p0[0] = (seq_t)1;               /* row 0 */
    p1[0] = (seq_t)1; p1[1] = (seq_t)1;            /* row 1 */
    p2[0] = (seq_t)2; p2[1] = (seq_t)5; p2[2] = (seq_t)1; /* row 2 */

    /* Compute rows from 3 up to r */
    for (idx_t cur = 3; cur <= r; ++cur) {
        /* Coefficients α, β, γ for this row */
        seq_t alpha = (seq_t)(3 * (seq_t)cur - 2);
        seq_t beta  = (seq_t)(3 * (seq_t)cur * (seq_t)cur
                              - 7 * (seq_t)cur + 4);
        seq_t tmp = (seq_t)(cur - 2);
        seq_t gamma = (seq_t)((seq_t)(cur - 1) * tmp * tmp);

        /* Compute current row p3, only up to index k (or cur if smaller) */
        idx_t jmax = (cur <= k) ? cur : k;
        for (idx_t j = 0; j <= jmax; ++j) {
            seq_t val = (seq_t)0;
            if (j > 0) val += p2[j - 1];
            val += alpha * p2[j];
            val -= beta * p1[j];
            val += gamma * p0[j];
            p3[j] = val;
        }

        /* Shift rows: p0<-p1, p1<-p2, p2<-p3, p3<-old p0 */
        seq_t *tmpPtr = p0;
        p0 = p1;
        p1 = p2;
        p2 = p3;
        p3 = tmpPtr;
    }

    seq_t result = p2[k];   /* row r now stored in p2 */

    /* Free allocated memory */
    free(p0);
    free(p1);
    free(p2);
    free(p3);

    return result;
}
