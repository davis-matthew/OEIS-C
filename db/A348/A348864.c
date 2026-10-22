/*
 * A348864  a(n) is the number of multiplications required to compute the
 *          permanent of general n X n matrices using trellis method with
 *          normalization.
 * https://oeis.org/A348864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*2^(n-1) - ceil(n/2)*binomial(n, floor(n/2)) + n^2 - n
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* handle offset starting at 1, but return 0 for n==0 just in case */
    if (n == 0) return 0;

    /* term1 = n * 2^(n-1) */
    seq_t term1 = (seq_t)n * pow2(n - 1);

    /* binomial part */
    idx_t floor_n2 = n / 2;
    idx_t ceil_n2   = (n + 1) / 2;
    seq_t binom     = binomial(n, floor_n2);
    seq_t term2     = (seq_t)ceil_n2 * binom;

    /* term3 = n^2 - n */
    seq_t term3 = ((seq_t)n * (seq_t)n) - (seq_t)n;

    return term1 - term2 + term3;
}
