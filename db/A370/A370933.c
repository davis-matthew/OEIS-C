/*
 * A370933  Number of pairs of antipodal vertices in the level n>1 Sierpiński
 *          triangle graph.
 * https://oeis.org/A370933
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * 2^(n-3) * ( 2^(n-2) + 3 ) for n >= 3 a(2) = 6
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the only special case directly */
    if (n == 2) {
        return (seq_t)6;
    }

    /* For n >= 3 use the closed‑form expression.
       Compute the two powers of two first. */
    seq_t p = pow2(n - 3);      /* 2^(n-3) */
    seq_t q = pow2(n - 2);      /* 2^(n-2) */

    /* Compute the remaining part of the formula. */
    seq_t sum  = q + (seq_t)3;  /* (2^(n-2) + 3) */
    seq_t prod = p * sum;       /* 2^(n-3) * (2^(n-2) + 3) */
    return (seq_t)3 * prod;     /* 3 * that product */
}
