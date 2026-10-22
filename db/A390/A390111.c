/*
 * A390111  a(n) is the total number of vertices of degree 1 in Catalan word
 *          grid graphs with n parts.
 * https://oeis.org/A390111
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*binom(2*n, n) + 5*binom(2*n-2, n-1) - binom(2*n+2,
 *            n+1)) / 2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A390111 */
/* Offset: 2 (a(2) = 4) */

/* Include the binomial function from the utilities */
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* The sequence is defined for n >= 2. For smaller n, return 0. */
    if (n < 2) return (seq_t)0;

    /* Compute the needed binomial coefficients.  All arithmetic on the
       indices is done in idx_t, and the results are in seq_t. */
    idx_t two_n        = n + n;            /* 2*n */
    idx_t two_n_minus2 = two_n - 2;        /* 2*n - 2 */
    idx_t two_n_plus2  = two_n + 2;        /* 2*n + 2 */

    seq_t term1 = 3 * binomial(two_n, n);
    seq_t term2 = 5 * binomial(two_n_minus2, n - 1);
    seq_t term3 = binomial(two_n_plus2, n + 1);

    seq_t result = term1 + term2 - term3;
    result /= 2;

    return result;
}
