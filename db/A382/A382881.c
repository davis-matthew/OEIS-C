/*
 * A382881  Triangle read by rows: T(n, k) = -Sum_{d|n, d<n} V(n, d)*T(d, k)
 *          for k >= 1, T(n, 0) = 0^n, T(n, n) = 1, where V(n, d) = 1 if d = 1
 *          otherwise valuation(n, d).
 * https://oeis.org/A382881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n‑th term of the triangle read by rows * * T(n,0) =
 *            0^n (1 if n == 0, 0 otherwise) * T(n,n) = 1 * T(n,k) = - Σ_{d|n,
 *            d<n} V(n,d) * T(d,k) for k >= 1 * * V(n,d) = 1 if d == 1 *
 *            valuation of n by d (largest e such that d^e | n) otherwise * *
 *            The sequence a(n) is simply the triangle T fl...
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for previously computed rows */
    static seq_t **triangle = NULL;   /* triangle[i][k] = T(i,k) */
    static idx_t rows = 0;            /* number of rows currently stored */

    /* determine row r and column k that contain the n‑th entry */
    idx_t r = 0;
    idx_t cumulative = 0;             /* elements before row r */
    while (1) {
        idx_t row_len = r + 1;
        if (n < cumulative + row_len)
            break;
        cumulative += row_len;
        ++r;
    }
    idx_t k = n - cumulative;         /* column inside row r */

    /* grow triangle if necessary */
    if (rows <= r) {
        idx_t old_rows = rows;
        rows = r + 1;
        triangle = (seq_t **)realloc(triangle,
                     sizeof(seq_t *) * rows);  /* cast needed for C++ */
        for (idx_t i = old_rows; i < rows; ++i) {
            triangle[i] = (seq_t *)malloc(sizeof(seq_t) * (i + 1)); /* cast */

            /* T(i,0) */
            if (i == 0)
                triangle[0][0] = (seq_t)1;
            else
                triangle[i][0] = (seq_t)0;    /* 0^i for i>0 */

            /* fill remaining columns */
            for (idx_t kk = 1; kk <= i; ++kk) {
                if (kk == i) {
                    triangle[i][kk] = (seq_t)1;   /* T(i,i) = 1 */
                    continue;
                }

                seq_t sum = (seq_t)0;

                /* enumerate proper divisors d of i */
                for (idx_t d = 1; d < i; ++d) {
                    if (i % d != 0)
                        continue;

                    /* compute V(i,d) */
                    seq_t v;
                    if (d == 1)
                        v = (seq_t)1;
                    else {
                        idx_t tmp = i;
                        uint64_t e = 0;
                        while (tmp % d == 0) {
                            tmp /= d;
                            ++e;
                        }
                        v = (seq_t)e;
                    }

                    /* only add if kk <= d (otherwise T(d,kk) is zero) */
                    if (kk <= d)
                        sum += v * triangle[d][kk];
                }

                triangle[i][kk] = -sum;
            }
        }
    }

    return triangle[r][k];
}
