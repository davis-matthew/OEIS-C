/*
 * A360708  Expansion of Sum_{k>=0} (x^2 / (1 - k*x))^k.
 * https://oeis.org/A360708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} k^(n-2*k) * binomial(n-k-1, k-1)
 *            with a(0) = 1
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 30, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 609 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;
    idx_t maxk = n / 2;
    for (idx_t k = 1; k <= maxk; ++k)
    {
        idx_t exp = n - 2 * k;          /* non‑negative, guaranteed */
        seq_t powk = ipow_idx((seq_t)k, exp);
        seq_t bin = binomial(n - k - 1, k - 1);
        sum += powk * bin;
    }
    return sum;
}
