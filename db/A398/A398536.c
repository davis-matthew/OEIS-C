/*
 * A398536  a(n) is the trace of the Hankel n X n matrix M_n whose M_n(i,j)
 *          generic element is n^(i+j).
 * https://oeis.org/A398536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^(2n+2)-n^2)/(n^2-1) for n>1; a(1)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the trace of the Hankel n×n matrix M_n(i,j)=n^(i+j). */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(1)=1 */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Convert n to the big integer type */
    seq_t bn = (seq_t)n;
    seq_t bn_sq = bn * bn;                /* n^2 */

    /* Exponent 2n+2 */
    idx_t tmp = n * 2ULL + 2ULL;          /* fits in idx_t for n up to 10000 */
    seq_t exp = (seq_t)tmp;

    /* Compute n^(2n+2) */
    seq_t pow_big = big_pow(bn, exp);

    /* Numerator = n^(2n+2) - n^2 */
    seq_t numerator = pow_big - bn_sq;

    /* Denominator = n^2 - 1 */
    seq_t denom = bn_sq - (seq_t)1;

    /* Exact division because the expression is known to be integral */
    seq_t result = numerator / denom;

    return result;
}
