/*
 * A262015  a(n) = [x^n] (1-x)^(4*n+1) * Sum_{k>=0} binomial(n+k-1,k)^4 * x^k.
 * https://oeis.org/A262015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A262015: 
   a(n) = Sum_{k=0..n} (-1)^{n-k} * binomial(n+k, k)^4 * binomial(4*n+1, n-k) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Empty sequence for negative indices is undefined; formula applies for n >= 0. */
    seq_t result = 0;
    idx_t m = 4 * n + 1;                       /* m = 4n+1 */
    /* Precompute binomial(m, j) for j = 0..n */
    seq_t *C2 = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!C2) return 0;                         /* Out of memory: return 0 as fallback */
    C2[0] = 1;
    for (idx_t j = 0; j < n; ++j) {
        C2[j + 1] = C2[j] * (seq_t)(m - j) / (seq_t)(j + 1);
    }

    seq_t c1 = 1;                               /* binomial(n+0,0) */
    for (idx_t k = 0; k <= n; ++k) {
        /* term = [binomial(n+k, k)]^4 * binomial(4n+1, n-k) */
        seq_t term = c1 * c1 * c1 * c1;          /* c1^4 */
        term = term * C2[n - k];
        if ((n - k) & 1)                       /* sign = (-1)^(n-k) */
            result -= term;
        else
            result += term;

        /* update c1 for next k: binomial(n+k+1, k+1) */
        if (k < n)
            c1 = c1 * (seq_t)(n + k + 1) / (seq_t)(k + 1);
    }

    free(C2);
    return result;
}
