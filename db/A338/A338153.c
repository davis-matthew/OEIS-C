/*
 * A338153  a(n) is the number of acyclic orientations of the edges of the
 *          n-prism.
 * https://oeis.org/A338153
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 + 7^n - 2^(n+1) - 2*4^n; offset 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n < 3) return (seq_t)0;   /* behaviour below offset is irrelevant */

    /* Compute the powers using the utility function */
    seq_t pw7 = big_pow((seq_t)7, (seq_t)n);          /* 7^n */
    seq_t pw2 = big_pow((seq_t)2, (seq_t)(n + 1));    /* 2^(n+1) */
    seq_t pw4 = big_pow((seq_t)4, (seq_t)n);          /* 4^n */

    /* 2*4^n */
    seq_t two_pw4 = pw4 * (seq_t)2;

    /* Assemble the formula */
    seq_t res = (seq_t)5;
    res += pw7;
    res -= pw2;
    res -= two_pw4;

    return res;
}
