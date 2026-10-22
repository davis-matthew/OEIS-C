/*
 * A378781  a(n) = n^3 * 2^n * binomial(3*n, n) / 3.
 * https://oeis.org/A378781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 * 2^n * binomial(3*n, n) / 3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute C(3n, n) and divide by 3 beforehand to prevent overflow. */
    seq_t binom = binomial(3 * n, n);
    binom /= 3;   /* exact division, valid for n ≥ 1 */

    /* 2^n */
    seq_t pow2n = pow2(n);

    /* n^3 */
    seq_t nv   = (seq_t)n;
    seq_t nsq  = nv * nv;
    seq_t ncub = nsq * nv;

    /* Assemble the product: binom * 2^n * n^3 */
    seq_t res = binom;
    res *= pow2n;
    res *= ncub;

    return res;
}
