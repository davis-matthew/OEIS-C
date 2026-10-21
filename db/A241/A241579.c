/*
 * A241579  Square array read by antidiagonals downwards: T(n,k) =
 *          Sum_{j=1..k} n^(k-j)*Stirling_2(k,j) (n >= 0, k >= 1).
 * https://oeis.org/A241579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{j=1..k} n^(k-j) * Stirling_2(k,j) a(n) = T(i,j)
 *            where (i,j) is obtained by reading the square array by
 *            antidiagonals downwards
 * Range:     u8 n <= 22, u16 n <= 46, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the diagonal D (with D>=1) such that
       C(D-1) <= n < C(D), where C(d)=d*(d+1)/2. */
    idx_t D = 0;
    while ((D * (D + 1) / 2) <= n)
        D++;
    idx_t prev = (D - 1) * D / 2;       /* C(D-1) */
    idx_t i    = n - prev;               /* row index, 0-based */
    idx_t j    = D - i;                  /* column index, >=1 */

    /* Compute Stirling numbers of the second kind for k = j. */
    seq_t *S = (seq_t *)malloc((j + 1) * sizeof(seq_t));
    seq_t *pow = (seq_t *)malloc((j + 1) * sizeof(seq_t));
    if (!S || !pow) { /* safety check */
        if (S) free(S);
        if (pow) free(pow);
        return 0;
    }

    /* Initialize S[0] = 1, all others 0 */
    S[0] = 1;
    for (idx_t k = 1; k <= j; ++k)
        S[k] = 0;

    /* Compute powers of i: pow[e] = i^e, e=0..j */
    pow[0] = 1;
    for (idx_t e = 1; e <= j; ++e)
        pow[e] = pow[e - 1] * (seq_t)i;   /* unsigned multiplication */

    /* Fill Stirling numbers iteratively */
    for (idx_t n_k = 1; n_k <= j; ++n_k) {
        for (idx_t k = n_k; k >= 1; --k) {
            /* S(k) = k*S(k) + S(k-1) */
            S[k] = k * S[k] + S[k - 1];
        }
        S[0] = 0;
    }

    /* Compute T(i,j) = sum_{t=1..j} i^(j-t) * S(t) */
    seq_t result = 0;
    for (idx_t t = 1; t <= j; ++t) {
        idx_t exp = j - t;          /* exponent of i */
        result += S[t] * pow[exp];
    }

    free(S);
    free(pow);
    return result;
}
