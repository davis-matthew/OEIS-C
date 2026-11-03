/*
 * A360824  Expansion of Sum_{k>0} (k * x)^k / (1 - k * x^k)^(k+1).
 * https://oeis.org/A360824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(d + n/d - 1) * binomial(d + n/d - 1, d)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0) continue;
        idx_t m  = n / d;
        idx_t s  = d + m - 1;

        seq_t pow_d = ipow((seq_t)d, (seq_t)s);
        seq_t bin_d = binomial(s, d);
        result += seq_mul(pow_d, bin_d);

        if (d != m)
        {
            seq_t pow_m = ipow((seq_t)m, (seq_t)s);
            seq_t bin_m = binomial(s, m);
            result += seq_mul(pow_m, bin_m);
        }
    }
    return result;
}
