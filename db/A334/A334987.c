/*
 * A334987  Sum of centered triangular numbers dividing n.
 * https://oeis.org/A334987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of centered triangular numbers dividing n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   is_centered_triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_centered_triangular.h"
/* Centered triangular numbers are given by C_k = 3*k*(k - 1)/2 + 1. */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            if (is_centered_triangular(d1))
                sum += (seq_t)d1;
            if (d2 != d1 && is_centered_triangular(d2))
                sum += (seq_t)d2;
        }
    }
    return sum;
}
