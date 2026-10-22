/*
 * A372918  a(n) = Sum_{k=1..n} gcd(k^3,n^2).
 * https://oeis.org/A372918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{k=1..n} gcd(k^3, n^2)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1633, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the sum directly using the supplied seq_t gcd routine. */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t result = (seq_t)0;
    seq_t n2 = (seq_t)n * n;          /* n^2, cast to seq_t first */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t k3 = (seq_t)k * k * k; /* k^3, cast to seq_t first */
        result += gcd(k3, n2);
    }
    return result;
}
