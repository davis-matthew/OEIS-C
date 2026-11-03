/*
 * A308675  a(n) = Sum_{d|n} d^(d^2 * n).
 * https://oeis.org/A308675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(d^2 * n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;

        /* first divisor d */
        __uint128_t exp_val = (__uint128_t)d * d * n;
        sum += ipow((seq_t)d, (seq_t)exp_val);

        /* paired divisor n/d, skip if equal to d */
        idx_t d2 = n / d;
        if (d2 != d) {
            exp_val = (__uint128_t)d2 * d2 * n;
            sum += ipow((seq_t)d2, (seq_t)exp_val);
        }
    }
    return sum;
}
