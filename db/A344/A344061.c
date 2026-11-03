/*
 * A344061  a(n) = Sum_{d|n} sigma(d)^(n/d).
 * https://oeis.org/A344061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} sigma(d)^(n/d)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 41, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d1;

            seq_t base1 = sigma_of(d1);
            seq_t exp1  = (seq_t)(n / d1);
            sum += ipow(base1, exp1);

            if (d1 != d2) {
                seq_t base2 = sigma_of(d2);
                seq_t exp2  = (seq_t)(n / d2);
                sum += ipow(base2, exp2);
            }
        }
    }
    return sum;
}
