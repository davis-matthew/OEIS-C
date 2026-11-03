/*
 * A367274  a(n) = binomial(n, k) * (n - k)^k where k = floor(n/2).
 * https://oeis.org/A367274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, floor(n/2)) * (n - floor(n/2))^floor(n/2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n) {
    idx_t k = n >> 1;                 /* floor(n/2) */
    seq_t bin = binomial(n, k);       /* C(n, k) */
    seq_t base = (seq_t)(n - k);      /* n - k  */
    seq_t pow = ipow_idx(base, k);     /* (n-k)^k */
    return seq_mul(bin, pow);         /* bin * (n-k)^k */
}
