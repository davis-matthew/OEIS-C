/*
 * A308367  Expansion of Sum_{k>=1} x^k/(1 + k*x^k).
 * https://oeis.org/A308367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (-d)^(n/d-1)
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 62, i64 n <= 122,
 *            i128 n <= 245, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;
            idx_t e1 = n / d1 - 1;
            seq_t term1 = big_pow(-((seq_t)d1), (seq_t)e1);
            sum += term1;
            if (d1 != d2) {
                idx_t e2 = n / d2 - 1;
                seq_t term2 = big_pow(-((seq_t)d2), (seq_t)e2);
                sum += term2;
            }
        }
    }
    return sum;
}
