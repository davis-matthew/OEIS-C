/*
 * A295520  a(n) is the least k >= 0 such that n XOR k is prime (where XOR
 *          denotes the bitwise XOR operator).
 * https://oeis.org/A295520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min{k ≥ 0 | n XOR k is prime}
 * Range:     u8 n <= 782, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least k such that n XOR k is prime. */

#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    for (idx_t k = 0; ; ++k) {
        seq_t x = val ^ (seq_t)k;
        if (is_prime_big(x)) {
            return (seq_t)k;
        }
    }
}
