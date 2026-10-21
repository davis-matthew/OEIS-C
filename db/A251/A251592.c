/*
 * A251592  Triangle of coefficients of polynomials P(n,t) related to the
 *          Mittag-Leffler function, where P(n,t) = Product_{k=0..n-2} n*t-k.
 * https://oeis.org/A251592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, changed
 * Formula:   a(n) = s(r-1, j) * r^j where r is the unique integer with
 *            r(r+1)/2 >= n, j = n - r(r-1)/2 - 1, and s(m,k) are the signed
 *            Stirling numbers of the first kind. The triangle lists the
 *            coefficients of P(r,t)=∏_{k=0}^{r-2}(r*t-k) in powers of t
 * Range:     i8 n <= 12, i16 n <= 24, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <cstddef>
#endif

seq_t a(idx_t n)
{
    if (n == 0) /* offset starts at 1 */
        return 0;

    /* --- determine row r and column j --- */
    idx_t r = 1;
    idx_t tri = 1;               /* r(r+1)/2 */
    while (tri < n) {
        ++r;
        tri += r;
    }
    idx_t tri_prev = tri - r;    /* r(r-1)/2 */
    idx_t j = n - tri_prev - 1;  /* 0‑based column */
    idx_t m = r - 1;             /* up to which we need Stirling numbers */

    /* --- compute signed Stirling numbers s(m,·) via DP --- */
    /* Allocate two arrays of size m+1 */
    seq_t *prev, *curr;
#ifdef __cplusplus
    prev = new seq_t[m + 1];
    curr = new seq_t[m + 1];
#else
    prev = (seq_t *)calloc(m + 1, sizeof(seq_t));
    curr = (seq_t *)calloc(m + 1, sizeof(seq_t));
#endif
    if (!prev || !curr) return 0; /* safety */

    prev[0] = 1;                  /* s(0,0)=1 */
    /* all other entries of prev already zero */
    for (idx_t i = 1; i <= m; ++i) {
        curr[0] = 0;
        for (idx_t k = 1; k <= i; ++k) {
            curr[k] = prev[k - 1] - (seq_t)(i - 1) * prev[k];
        }
        /* swap prev and curr */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t s_val = prev[j];        /* s(m,j) */

    /* --- compute r^j --- */
    seq_t pow = 1;
    for (idx_t t = 0; t < j; ++t)
        pow *= (seq_t)r;

    /* --- clean up --- */
#ifdef __cplusplus
    delete[] prev;
    delete[] curr;
#else
    free(prev);
    free(curr);
#endif

    return s_val * pow;
}
