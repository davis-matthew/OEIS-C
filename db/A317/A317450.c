/*
 * A317450  a(n)=(-1)^((n-2)*(n-1)/2)*2^((n-1)^2)*n^(n-3).
 * https://oeis.org/A317450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^((n-2)*(n-1)/2) * 2^((n-1)^2) * n^(n-3)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 5, i64 n <= 7, i128 n <= 10,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle offset: first index is 1 */
    if (n == 1 || n == 2) {
        return (seq_t)1;
    }

    /* Compute 2^((n-1)^2) */
    idx_t n_minus1 = n - 1;
    seq_t exp2 = (seq_t)n_minus1 * (seq_t)n_minus1;   /* (n-1)^2 */
    seq_t pow2 = big_pow((seq_t)2, exp2);

    /* Compute n^(n-3) */
    seq_t expn = (seq_t)(n - 3);                       /* n-3 */
    seq_t pown = big_pow((seq_t)n, expn);

    /* Multiply the two parts */
    seq_t result = pow2 * pown;

    /* Apply sign: (-1)^((n-2)*(n-1)/2) */
    idx_t sign_exp = ((n - 2) * (n - 1)) / 2;
    if (sign_exp & 1) {
        result = -result;
    }

    return result;
}
