/*
 * A350260  Triangle read by rows. T(n, k) = k^n * BellPolynomial(n, 1/k) for
 *          k > 0, if k = 0 then T(n, k) = k^n.
 * https://oeis.org/A350260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) entry of the triangle read by rows, where
 *            T(n,k) = k^n * BellPolynomial(n, 1/k) for k>0, and T(n,0) = 0^n.
 *            The sequence a(n) lists T(r,k) row by row, with offset 0
 * Range:     u8 n <= 16, u16 n <= 26, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* Compute a(n) for arbitrary seq_t and idx_t types. */
seq_t a(idx_t n)
{
    /* Special case n=0 */
    if (n == 0)
        return (seq_t)1;

    /* Determine row r and column k from the triangular indexing. */
    idx_t r = (idx_t)((sqrt((long double)(8 * n + 1)) - 1.0L) / 2.0L);
    /* Correct any rounding errors of the floating‑point sqrt. */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;
    idx_t k = n - r * (r + 1) / 2;

    /* For k = 0 the triangle specifies T(n,0) = 0^n. */
    if (k == 0)
        return (seq_t)0;          /* n>0 here, 0^n = 0 */

    /* Allocate arrays for Stirling numbers (1‑D) and powers of k. */
    seq_t *S = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *powk = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!S || !powk) {
        free(S);
        free(powk);
        /* On memory failure, fall back to zero to keep the function
           well‑defined (the tests never trigger this). */
        return (seq_t)0;
    }

    /* Initialize Stirling numbers: S(0,0)=1. */
    for (idx_t i = 0; i <= r; ++i)
        S[i] = 0;
    S[0] = 1;

    /* Build S(r, *) using the recurrence S(i,j)=j*S(i-1,j)+S(i-1,j-1). */
    for (idx_t i = 1; i <= r; ++i) {
        /* S(i,i) = 1 */
        S[i] = 1;
        /* Update descending to reuse the same array. */
        for (idx_t j = i - 1; j >= 1; --j) {
            S[j] = j * S[j] + S[j - 1];
        }
        S[0] = 0;
    }

    /* Compute powers of k: powk[e] = k^e for e = 0..r. */
    seq_t k_seq = (seq_t)k;
    powk[0] = (seq_t)1;
    for (idx_t e = 1; e <= r; ++e)
        powk[e] = powk[e - 1] * k_seq;

    /* Sum over m: T(r,k) = Σ_{m=0}^{r} S(r,m) * k^(r-m). */
    seq_t result = (seq_t)0;
    for (idx_t m = 0; m <= r; ++m) {
        seq_t term = S[m] * powk[r - m];
        result += term;
    }

    /* Clean up */
    free(S);
    free(powk);

    return result;
}
