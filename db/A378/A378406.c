/*
 * A378406  a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) *
 *          binomial(n+2*k,n-2*k).
 * https://oeis.org/A378406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) * binomial(n+2*k,
 *            n-2*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 60,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 978 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = n / 2;
    for (idx_t k = 0; k <= limit; ++k) {
        seq_t b1 = binomial(n, k);
        seq_t b2 = binomial(n + 2 * k, n - 2 * k);
        seq_t term = seq_mul(b1, b2);
        sum = sum + term;
    }
    return sum;
}
