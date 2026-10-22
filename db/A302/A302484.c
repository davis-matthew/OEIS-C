/*
 * A302484  Number of Truchet tilings of an n X n square up to rotation and
 *          reflection.
 * https://oeis.org/A302484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^(n^2) + 5*4^(n^2/2) + 2*4^(n^2/4)) / 8 if n even *
 *            a(n) = (4^(n^2) + 2*4^(n^2/2)) / 8 if n odd * * 4^(n^2/2) equals
 *            2^(n^2) for all n. * 4^(n^2/4) equals 2^(n^2/2) when n is even
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t n_sq = (seq_t)n * (seq_t)n;          /* n^2 */
    if ((n & 1) == 0) {                       /* even n */
        seq_t term0 = big_pow(4, n_sq);       /* 4^(n^2) */
        /* 4^(n^2/2) = 2^(n^2) */
        seq_t term1 = 5 * big_pow(2, n_sq);   /* 5 * 4^(n^2/2) */
        seq_t term2 = 2 * big_pow(2, n_sq / 2);/* 2 * 4^(n^2/4) */
        return (term0 + term1 + term2) / 8;   /* division truncates as integer */
    } else {                                   /* odd n */
        seq_t term0 = big_pow(4, n_sq);       /* 4^(n^2) */
        seq_t term1 = 2 * big_pow(2, n_sq);   /* 2 * 4^(n^2/2) = 2 * 2^(n^2) */
        return (term0 + term1) / 8;
    }
}
