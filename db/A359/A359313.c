/*
 * A359313  Triangular array read by rows. T(n,k) is the number of Green's
 *          H-classes contained in the D-class of rank k matrices in the
 *          semigroup Mat_n(F_2) of n X n matrices over the field F_2. n>=0,
 *          0<=k<=n.
 * https://oeis.org/A359313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangular array read by rows of squared GF(2) *
 *            Gaussian binomial coefficients: * * Write the global index N as
 *            N = r(r+1)/2 + k with 0 <= k <= r. * Then a(N) = ([r choose
 *            k]_2)^2 where * * [r choose k]_2 = ∏_{i=1..k} ( 2^{r+1-i} - 1 )
 *            / ( 2^i - 1 ) * * The value [r choose k]_2 is an...
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 38, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Compute the Gaussian binomial coefficient [n choose k]_2. */
static seq_t gaussian_binom(idx_t n, idx_t k)
{
    /* Use symmetry to minimise k. */
    if (k > n - k)
        k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = pow2(n + 1 - i);
        num -= 1;                 /* 2^{n+1-i} - 1 */
        seq_t den = pow2(i);
        den -= 1;                 /* 2^i - 1 */
        res = (res * num) / den;
    }
    return res;
}

/* Global sequence function for OEIS A359313. */
seq_t a(idx_t N)
{
    /* Find the row r such that r(r+1)/2 <= N < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= N)
        ++r;

    /* Column index within the row. */
    idx_t k = N - r * (r + 1) / 2;

    /* Compute the squared Gaussian binomial coefficient. */
    seq_t g = gaussian_binom(r, k);
    return g * g;
}
