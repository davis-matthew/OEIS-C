/*
 * A360817  Expansion of Sum_{k>=0} (k*x)^(3*k) / (1 - k*x)^(k+1).
 * https://oeis.org/A360817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} k^n * binomial(n-2*k, k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 412 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t pow = ipow((seq_t)k, (seq_t)n);
        seq_t bin = binomial(n - 2 * k, k);
        seq_t term = seq_mul(pow, bin);
        sum += term;
    }
    return sum;
}
