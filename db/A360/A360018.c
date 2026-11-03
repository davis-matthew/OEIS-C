/*
 * A360018  Expansion of Sum_{k>=0} (k * x * (1 + (k * x)^2))^k.
 * https://oeis.org/A360018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (n-2*k)^n * binomial(n-2*k, k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;            /* floor(n/3) */

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t base_idx = n - 2 * k;           /* base as idx_t */
        seq_t base = (seq_t)base_idx;

        /* Compute base^n (with 0^0 treated as 1) */
        seq_t pow_term;
        if (base == 0 && n == 0)
            pow_term = 1;
        else
            pow_term = ipow_i32(base, (int)n);

        /* binomial coefficient C(base, k) */
        seq_t bin = binomial(base_idx, k);

        /* Add term to the sum */
        sum += pow_term * bin;
    }

    return sum;
}
