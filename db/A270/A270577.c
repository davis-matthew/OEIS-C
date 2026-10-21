/*
 * A270577  Generalized Catalan numbers C(3,n), where the (m,n)-th Catalan is
 *          the number of paths in R^m from the origin to the point
 *          (n,...,n,(m-1)n) with m kinds of moves such that the path never
 *          rises above the hyperplane x_m = x_1+...+x_{m-1}.
 * https://oeis.org/A270577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = catalan(2*n) * binomial(2*n, n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t c = catalan(2 * n);        /* Catalan(2n) */
    seq_t b = binomial(2 * n, n);    /* binom(2n, n) */
    return c * b;
}
