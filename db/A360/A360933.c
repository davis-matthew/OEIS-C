/*
 * A360933  Expansion of e.g.f. Sum_{k>=0} exp((3^k - 1)*x) * x^k/k!.
 * https://oeis.org/A360933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (3^k - 1)^(n-k) * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   binomial, ipow_idx, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* base = 3^k - 1 */
        seq_t base = pow3(k);
        base -= (seq_t)1;

        /* pow_part = (3^k - 1)^(n-k) */
        seq_t pow_part = ipow_idx(base, n - k);

        /* bin = binomial(n, k) */
        seq_t bin = binomial(n, k);

        /* add term to total */
        total += pow_part * bin;
    }
    return total;
}
