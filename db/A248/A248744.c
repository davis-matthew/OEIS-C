/*
 * A248744  Number of different ways one can attack all squares on an n X n
 *          chessboard with n rooks.
 * https://oeis.org/A248744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n^n - n! (n >= 0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* 2*0^0 - 0! = 2*1 - 1 = 1 */
        return (seq_t)1;
    }

    /* Compute n^n as a big integer */
    seq_t pow_val = big_pow((seq_t)n, (seq_t)n);

    /* Compute n! as a big integer */
    seq_t fact_val = factorial(n);

    /* Compute 2*n^n - n! */
    const seq_t two = (seq_t)2;
    return two * pow_val - fact_val;
}
