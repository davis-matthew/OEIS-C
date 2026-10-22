/*
 * A358119  a(n) = Sum_{j=0..n} (-1)^j*binomial(2*n - j, j)*c(n - j)*c(n - j +
 *          2), where c(n) is the n-th Catalan number.
 * https://oeis.org/A358119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} (-1)^j*binomial(2*n - j, j)*c(n - j)*c(n - j
 *            + 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the closed‑form definition using only integer arithmetic. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* ---------------  Catalan numbers up to n+2  --------------- */
    size_t size = (size_t)(n + 3);                 /* indices 0 … n+2 inclusive */
    seq_t *cat = (seq_t *)malloc(size * sizeof(seq_t));
    if (!cat) return (seq_t)0;                     /* out of memory → zero */

    cat[0] = (seq_t)1;                            /* C0 = 1 */
    for (idx_t k = 1; k <= n + 2; ++k) {
        seq_t num = cat[k - 1] * (seq_t)(4 * k - 2); /* Ck = Ck-1 * (4k-2)/(k+1) */
        seq_t den = (seq_t)(k + 1);
        cat[k] = num / den;                       /* exact division */
    }

    /* ---------------  Alternating sum  --------------- */
    seq_t pos = (seq_t)0;                          /* sum of even‑j terms */
    seq_t neg = (seq_t)0;                          /* sum of odd‑j terms */
    seq_t binom = (seq_t)1;                        /* binomial(2*n,0) */

    for (idx_t j = 0; j <= n; ++j) {
        idx_t k = n - j;                           /* index for Catalan terms */

        seq_t term = binom * cat[k] * cat[k + 2];
        if ((j & 1) == 0) pos += term;            /* even j */
        else             neg += term;            /* odd j  */

        if (j == n) break;                        /* no next binomial needed */

        /*  Compute binom_{j+1} = binom_j * (2n-2j)*(2n-2j-1) / ((j+1)*(2n-j)) */
        seq_t num1   = (seq_t)(2 * n - 2 * j);
        seq_t num2   = num1 - 1;
        seq_t denom1 = (seq_t)(j + 1);
        seq_t denom2 = (seq_t)(2 * n - j);
        seq_t denom   = denom1 * denom2;

        binom = binom * num1;
        binom = binom * num2;
        binom = binom / denom;                    /* exact division */
    }

    /* ---------------  Final result  --------------- */
    seq_t result = pos - neg;                      /* pos ≥ neg for all n */
    free(cat);
    return result;
}
