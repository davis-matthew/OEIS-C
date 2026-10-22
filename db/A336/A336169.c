/*
 * A336169  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          T(n,k) = Sum_{j=0..n} (-1)^(n-j) * multinomial(n+(k-1)*j; n-j,
 *          {j}^k).
 * https://oeis.org/A336169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value on the antidiagonal read of the square array
 *            T(n,k) where T(n,k) = Σ_{j=0..n} (-1)^{n-j} *
 *            multinomial(n+(k-1)j; n-j, j^k)
 * Range:     u8 n <= 22, u16 n <= 29, u32 n <= 47, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Required utilities:  binom() for binomial coefficients.
    All other operations are performed on the arbitrary-precision type seq_t.
*/

#include "../utils/binom.h"

/*  Compute the multinomial term for given n,k,l:
        N = n + (k-1)*l
        term = C(N, n-l) * Π_{t=0}^{k-1} C(N-(n-l)-t*l, l)
*/
static seq_t multinomial_term(idx_t n, idx_t k, idx_t l)
{
    seq_t term = binom(n + (k-1)*l, n - l);   /* C(N, n-l)               */

    /* Multiply by the product of binomials for the k identical groups of size l */
    idx_t remaining = k * l;
    for (idx_t t = 0; t < k; ++t)
    {
        term *= binom(remaining, l);
        remaining -= l;
    }
    return term;
}

/*  Compute T(n,k) by summing the alternating multinomial terms   */
static seq_t compute_T(idx_t n, idx_t k)
{
    seq_t pos = 0, neg = 0;      /* accumulate positive and negative parts separately */

    for (idx_t l = 0; l <= n; ++l)
    {
        seq_t term = multinomial_term(n, k, l);

        if (((n - l) & 1) == 0)      /* even exponent => positive sign */
            pos += term;
        else
            neg += term;
    }

    /* final result is non‑negative */
    return (pos >= neg) ? (pos - neg) : (neg - pos);
}

/*  Main entry function:  a(n) = T(i,j) where i+j = s and a(n) enumerates the array by anti‑diagonals   */
seq_t a(idx_t n)
{
    /* Find the diagonal index s such that :
         s*(s+1)/2 ≤ n < (s+1)*(s+2)/2
       (the number of terms up to diagonal s is (s+1)(s+2)/2 )
    */
    idx_t s = 0;
    while ( (s + 1) * (s + 2) / 2 <= n )
        ++s;

    idx_t start = s * (s + 1) / 2;   /* first index of diagonal s   */
    idx_t j = n - start;              /* position inside the diagonal */
    idx_t k = s - j;                  /* corresponding column index   */

    return compute_T(j, k);
}
