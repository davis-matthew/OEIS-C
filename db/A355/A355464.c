/*
 * A355464  Expansion of Sum_{k>=0} x^k/(1 - k^k * x)^(k+1).
 * https://oeis.org/A355464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k^(k*(n-k)) * binomial(n,k)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t power;
        if (k == 0)
            power = 1; /* 0^(0) = 1 by convention */
        else
            power = ipow((seq_t)k, (seq_t)k * (seq_t)(n - k));

        seq_t bin = binomial(n, k);
        seq_t term = power * bin; /* use builtin *; works for native and big ints */
        sum += term;
    }
    return sum;
}
