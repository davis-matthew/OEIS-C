/*
 * A398479  a(n) = (2*n)^n * binomial(3*n, n) / (n + 1). Central terms of
 *          A398477.
 * https://oeis.org/A398479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n)^n * binomial(3*n, n) / (n + 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* (2*n)^n */
    seq_t base = (seq_t)(n * 2);
    seq_t pow_term = seq_pow(base, n);

    /* binomial(3*n, n) */
    seq_t bin_term = binomial((idx_t)(3 * n), (idx_t)n);

    /* multiply and divide by (n + 1) */
    seq_t result = pow_term * bin_term;
    result /= ((seq_t)n + 1);
    return result;
}
