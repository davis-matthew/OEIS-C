/*
 * A368513  a(n) = (6*n+1)!/(n!*(2*n)!*(3*n+1)!).
 * https://oeis.org/A368513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6*n+1)!/(n!*(2*n)!*(3*n+1)!)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent combinatorial expression: a(n) = C(6*n+1, n) * C(5*n+1, 2*n) */

#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;                    /* handles the offset 0 */

    /* compute C(6*n+1, n) */
    idx_t six_n_plus1 = (idx_t)6 * n + 1;
    seq_t t1 = binomial(six_n_plus1, n);

    /* compute C(5*n+1, 2*n) */
    idx_t five_n_plus1 = (idx_t)5 * n + 1;
    seq_t t2 = binomial(five_n_plus1, 2 * n);

    /* multiply the two binomial coefficients */
    return seq_mul(t1, t2);
}
