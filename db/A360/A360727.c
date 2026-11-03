/*
 * A360727  Expansion of Sum_{k>=0} (k * x * (1 + x^2))^k.
 * https://oeis.org/A360727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (n-2*k)^(n-2*k) * binomial(n-2*k,
 *            k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 380 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 0; k <= n / 3; ++k) {
        idx_t t = n - 2 * k;          /* t = n-2k */
        seq_t base = (seq_t)t;
        /* 0^0 is handled as 1 by big_pow – if not, the explicit check below
           would replace it. */
        seq_t power = (t == 0) ? 1 : ipow(base, base);
        seq_t bin = binomial(t, k);
        result += power * bin;
    }
    return result;
}
