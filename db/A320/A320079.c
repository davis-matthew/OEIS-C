/*
 * A320079  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of e.g.f. 1/(1 + k*log(1 - x)).
 * https://oeis.org/A320079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{j=1..i} |Stirling1(i,j)| * j! * k^j , where (i,j)
 *            are obtained from the index n by reading the square array A(i,j)
 *            in antidiagonal order
 * Range:     u8 n <= 24, u16 n <= 41, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ----- 1. Map the linear index n to (i,j) ----- */
    size_t d = 0;
    /* Find smallest d such that (d+1)(d+2)/2 > n */
    while ( ((d + 1) * (d + 2)) / 2 <= n )
        d++;
    /* i counts along the diagonal, j = d - i */
    size_t i = (size_t)n - (d * (d + 1)) / 2;
    size_t j = d - i;

    /* ----- 2. Base case ----- */
    if (i == 0)
        return (seq_t)1;          /* A(0, k) = 1 for all k */

    /* ----- 3. Compute unsigned Stirling numbers of the first kind up to i ----- */
    size_t N = i;
    size_t sz = (N + 1) * (N + 1);
    seq_t *S = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!S)
        return (seq_t)0;          /* memory failure; unlikely for small N */

    /* Initialise to 0 */
    for (size_t idx = 0; idx < sz; ++idx)
        S[idx] = (seq_t)0;

    /* Base row */
    S[0] = (seq_t)1;

    /* Build rows */
    for (size_t nrow = 1; nrow <= N; ++nrow) {
        S[nrow * (N + 1) + 0] = (seq_t)0;
        for (size_t kcol = 1; kcol <= nrow; ++kcol) {
            seq_t a1 = S[(nrow - 1) * (N + 1) + (kcol - 1)];
            seq_t a2 = S[(nrow - 1) * (N + 1) + kcol] * (seq_t)(nrow - 1);
            S[nrow * (N + 1) + kcol] = a1 + a2;
        }
    }

    /* ----- 4. Compute factorials up to N ----- */
    seq_t *fact = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!fact) {
        free(S);
        return (seq_t)0;
    }
    fact[0] = (seq_t)1;
    for (size_t t = 1; t <= N; ++t)
        fact[t] = fact[t - 1] * (seq_t)t;

    /* ----- 5. Accumulate the sum ----- */
    seq_t res = (seq_t)0;
    seq_t k_pow = (seq_t)1;          /* k^0 */
    for (size_t l = 1; l <= N; ++l) {
        k_pow = k_pow * (seq_t)j;    /* k^l */
        seq_t term = S[N * (N + 1) + l] * fact[l] * k_pow;
        res += term;
    }

    /* ----- 6. Free temporary storage ----- */
    free(S);
    free(fact);

    return res;
}
