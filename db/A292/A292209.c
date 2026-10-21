/*
 * A292209  Number of cliques in the n-Menger sponge graph.
 * https://oeis.org/A292209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*20^n - 2*8^n + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(n) = 3*20^n - 2*8^n + 1, for n >= 1 (offset 1) */
    seq_t pow20 = big_pow((seq_t)20, (seq_t)n);
    seq_t pow8  = big_pow((seq_t)8,  (seq_t)n);

    seq_t result = pow20 * (seq_t)3;            /* 3 * 20^n */
    seq_t sub     = pow8  * (seq_t)2;           /* 2 * 8^n */
    result = result - sub;                      /* 3*20^n - 2*8^n */
    result = result + (seq_t)1;                 /* + 1 */

    return result;
}
