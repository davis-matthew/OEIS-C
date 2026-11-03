/*
 * A360814  Expansion of Sum_{k>=0} x^(2*k) / (1 - k*x)^(k+1).
 * https://oeis.org/A360814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} k^(n-2*k) * binomial(n-k,k)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        seq_t pw   = ipow_idx((seq_t)k, n - 2 * k);
        seq_t bin  = binomial(n - k, k);
        seq_t term = seq_mul(pw, bin);
        total += term;
    }
    return total;
}
