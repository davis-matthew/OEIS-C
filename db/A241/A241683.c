/*
 * A241683  Total number of 2 X 2 squares appearing in the Thue-Morse sequence
 *          logical matrices after n stages.
 * https://oeis.org/A241683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4^n - 2*2^n - 8)/18 for n even, * a(n) = (4^n - 4*2^n +
 *            4)/18 for n odd * a(0) = 0 * (This is equivalent to a(n) =
 *            (1/18)*(4^n - 3*2^n - 6*(-1)^n + (-2)^n - 2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);   /* 2^n */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);   /* 4^n */
    seq_t numerator;

    if ((n & 1) == 0) {                       /* n even */
        /* pow4 - 2*2^n - 8  (pow4 >= 2*pow2 for n >= 2) */
        numerator = pow4 - (pow2 * (seq_t)2);
        numerator -= (seq_t)8;
    } else {                                   /* n odd */
        /* (4^n + 4) - 4*2^n  (pow4 + 4 >= 4*pow2 for n >= 1) */
        numerator = pow4 + (seq_t)4;
        numerator -= (pow2 * (seq_t)4);
    }

    return numerator / (seq_t)18;
}
