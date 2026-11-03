/*
 * A384197  The Barrett reducer reciprocal mod n.
 * https://oeis.org/A384197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(4^k / n) where k = floor(log2(n)) + 1
 * Range:     u8 n <= 7, u16 n <= 853, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 80 terms through n = 7550 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

/* Computes the Barrett reducer reciprocal modulo n */
seq_t a(idx_t n)
{
    /* n >= 1 as per sequence definition */
    idx_t k = 0;
    idx_t t = n;
    while (t > 0) {      /* count bits of n → floor(log2(n)) + 1 */
        t >>= 1;
        ++k;
    }
    /* 4^k is 2^(2k); use the pow4 utility for arbitrary precision */
    return pow4(k) / (seq_t)n;
}
