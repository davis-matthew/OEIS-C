/*
 * A381624  For any nonnegative integer n with ternary expansion Sum_{k >= 0}
 *          t_k * 3^k, a(n) = Sum_{k >= 0} t_k * (2^(k+1) - 1).
 * https://oeis.org/A381624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_k t_k * (2^(k+1) - 1) where the t_k are the base‑3
 *            digits of n
 * Range:     u8 n <= 1103, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t k = 0;
    while (n) {
        idx_t digit = n % 3;
        n /= 3;
        seq_t weight = pow2(k + 1);          /* 2^(k+1) */
        weight -= 1;                         /* (2^(k+1) - 1) */
        sum += (seq_t)digit * weight;
        k++;
    }
    return sum;
}
