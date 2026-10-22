/*
 * A308579  a(n) = (9*2^n - 6*n - 10)/2.
 * https://oeis.org/A308579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*2^n - 6*n - 10)/2
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Computes the n-th term of OEIS A308579
 * offset: 1  (so a(1) is the first listed term) */
seq_t a(idx_t n)
{
    /* For n < 1 the sequence is not defined in OEIS; return 0 */
    if (n < 1) return 0;

    /* Compute 2^n as seq_t */
    seq_t two_pow_n = pow2(n);

    /* num = 9*2^n - 6*n - 10 */
    seq_t num = 9 * two_pow_n;
    num -= (seq_t)6 * ((seq_t)n);
    num -= 10;

    /* Divide by 2 */
    num /= 2;

    return num;
}
