/*
 * A328749  a(n) = Sum_{k = 0..w and t_k > 0} (-1)^t_k * 2^k, where Sum_{k =
 *          0..w} t_k * 3^k is the ternary representation of n.
 * https://oeis.org/A328749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = Sum_{k = 0..w and t_k > 0} (-1)^t_k * 2^k, where n =
 *            Sum_{k = 0..w} t_k * 3^k is the ternary representation of n
 * Range:     i8 n <= 2187, i16 n <= 6561, i32 n <= 6561, i64 n <= 6561,
 *            i128 n <= 6561, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    seq_t res = 0;
    idx_t k = 0;
    idx_t m = n;
    while (m != 0) {
        idx_t digit = m % 3;
        m /= 3;
        if (digit == 1) {
            res -= pow2(k);
        } else if (digit == 2) {
            res += pow2(k);
        }
        ++k;
    }
    return res;
}
