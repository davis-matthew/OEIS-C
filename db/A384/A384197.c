/*
 * A384197  The Barrett reducer reciprocal mod n.
 * https://oeis.org/A384197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(4^k / n) where k = floor(log2(n)) + 1
 * Range:     u8 n <= 7, u16 n <= 853, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow4.h"

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
