/*
 * A384415  a(n) = 4^n - 3^n - n*3^(n-1) - binomial(n,2)*3^(n-2).
 * https://oeis.org/A384415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 3^n - n*3^(n-1) - binomial(n,2)*3^(n-2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* compute main powers */
    seq_t pow4 = pow_uint(4, n);
    seq_t pow3 = pow_uint(3, n);

    /* start with 4^n - 3^n */
    seq_t result = pow4 - pow3;

    /* subtract n*3^(n-1) if n >= 1 */
    if (n >= 1) {
        seq_t p3_n_minus_1 = pow_uint(3, n-1);
        seq_t term = ((seq_t)n) * p3_n_minus_1;
        result -= term;
    }

    /* subtract binomial(n,2)*3^(n-2) if n >= 2 */
    if (n >= 2) {
        seq_t coeff = binomial(n, 2);          /* n*(n-1)/2 */
        seq_t p3_n_minus_2 = pow_uint(3, n-2);
        seq_t term = coeff * p3_n_minus_2;
        result -= term;
    }

    return result;
}
