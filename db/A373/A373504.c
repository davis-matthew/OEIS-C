/*
 * A373504  Triangular array: row n gives the coefficients T(n,k) of powers
 *          x^(2k) in the series expansion of ((b^n + b^(-n))/2)^2, where b =
 *          x + sqrt(x^2 + 1).
 * https://oeis.org/A373504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flattened row‑major array of T(r,k) where T(r,0)=1
 *            T(r,k)=4^(k-1)*(2*binomial(r+k,2k)-binomial(r+k-1,2k-1)) for k>0
 *            with r and k derived from index n as: r = the largest integer
 *            with r(r+1)/2 ≤ n k = n – r(r+1)/2
 * Range:     u8 n <= 17, u16 n <= 40, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binomial entries are provided by the utilities.  */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r and column k from the flat index n. */
    idx_t r = 0;
    idx_t cum = 0;                     /* number of terms before row r */
    while (cum + r + 1 <= n) {
        cum += r + 1;
        ++r;
    }
    idx_t k = n - cum;
    if (k == 0)
        return (seq_t)1;

    /* Compute the required binomial coefficients. */
    seq_t term1 = binomial(r + k, 2 * k);          /* C(r+k, 2k)   */
    term1 *= (seq_t)2;                              /* 2 * term1    */
    seq_t term2 = binomial(r + k - 1, 2 * k - 1);   /* C(r+k-1, 2k-1) */
    seq_t term = term1 - term2;                     /* 2*C - C'      */

    /* Multiply by 4^(k-1) = 2^(2k-2). */
    term <<= (unsigned)(2 * (k - 1));

    return term;
}
