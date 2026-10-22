/*
 * A358735  Triangular array read by rows. T(n, k) is the coefficient of x^k
 *          in a(n+3) where a(1) = a(2) = a(3) = 1 and a(m+2) = (m*x +
 *          2)*a(m+1) - a(m) for all m in Z.
 * https://oeis.org/A358735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) is the (n)-th element of the triangular array read by
 *            rows. * The array T(n,k) consists of the coefficients of x^k in
 *            the * polynomial a_{n+3}(x) defined by * a_1 = a_2 = a_3 = 1 *
 *            a_m = ((m-3)*x + 2)·a_{m-1} – a_{m-2} for m ≥ 4 * Hence a(n) =
 *            T(⌊(√(1+8n)-1)/2⌋ , n-⌊(⌊(√(1+8n)-1)/2⌋)(⌊(√(1...
 * Range:     u8 n <= 17, u16 n <= 39, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* External types supplied by the build system */
extern seq_t a(idx_t n)
{
    /* ----- locate the row ---- */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    idx_t start = (r * (r + 1)) / 2;
    size_t k = (size_t)(n - start);          /* position inside the row */

    /* ----- prepare the polynomials a_m(x) ---- */
    size_t target_m = (size_t)(r + 3);       /* we need a_{r+3} */

    static seq_t **poly  = NULL;
    static size_t *len   = NULL;             /* number of coefficients per m */
    static size_t max_computed = 0;          /* highest m already built */

    if (max_computed < target_m) {
        size_t old = max_computed;
        size_t newmax = target_m;

        /* grow the array of pointers */
        poly = (seq_t **)realloc(poly, (newmax + 1) * sizeof(seq_t *));
        len  = (size_t *)realloc(len,  (newmax + 1) * sizeof(size_t));

        /* build missing polynomials */
        for (size_t i = old + 1; i <= newmax; ++i) {
            if (i == 1 || i == 2 || i == 3) {
                len[i] = 1;
                poly[i] = (seq_t *)malloc(sizeof(seq_t));
                poly[i][0] = (seq_t)1;
            } else {
                size_t deg = i - 3;              /* degree of a_i */
                len[i] = deg + 1;
                poly[i] = (seq_t *)malloc(len[i] * sizeof(seq_t));

                seq_t mult = (seq_t)(i - 3);     /* coefficient of x inside ((i-3)*x+2) */

                for (size_t j = 0; j <= deg; ++j) {
                    seq_t val = 0;

                    /* 2 * a_{i-1}[j] */
                    if (j < len[i-1])
                        val += (seq_t)2 * poly[i-1][j];

                    /* (i-3) * a_{i-1}[j-1] */
                    if (j > 0 && j-1 < len[i-1])
                        val += mult * poly[i-1][j-1];

                    /* - a_{i-2}[j] */
                    if (j < len[i-2])
                        val -= poly[i-2][j];

                    poly[i][j] = val;
                }
            }
        }
        max_computed = newmax;
    }

    /* ----- return the requested coefficient ----- */
    return poly[target_m][k];
}
