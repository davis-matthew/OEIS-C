/*
 * A254464  a(n) = 21*2^n + 10*4^n + 15*3^n + 3*6^n + 6*5^n + 7^n + 28.
 * https://oeis.org/A254464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*2^n + 10*4^n + 15*3^n + 3*6^n + 6*5^n + 7^n + 28
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   pow2, pow3, pow4, power5, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"
#include "../utils/power5.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    seq_t p2 = pow2(n);          /* 2^n */
    seq_t p3 = pow3(n);          /* 3^n */
    seq_t p4 = pow4(n);          /* 4^n */
    seq_t p5 = power5(n);        /* 5^n */
    seq_t p7 = seq_pow((seq_t)7, n);  /* 7^n */
    seq_t p6 = p2 * p3;          /* 6^n = 2^n * 3^n */

    return 21 * p2 + 10 * p4 + 15 * p3 + 3 * p6 + 6 * p5 + p7 + 28;
}
