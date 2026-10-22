/*
 * A304339  Fixed point of f starting with n, where f(x) = x/(largest perfect
 *          power divisor of x).
 * https://oeis.org/A304339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   max_perfect_power_divisor, uint64_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A304339: fixed point of f(n) where
   f(x) = x / (largest perfect power divisor of x). The function
   repeatedly applies f until a fixed point (no perfect power
   divisor > 1) is reached. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/max_perfect_power_divisor.h"
#include "../utils/uint64_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety, though n >= 1 by specification */

    seq_t cur = (seq_t)n;
    while (true) {
        uint64_t cur64 = (uint64_t)cur;          /* cur fits in 64 bits for this sequence */
        seq_t d = max_perfect_power_divisor(cur64);
        if (d == (seq_t)1) return cur;          /* fixed point reached */
        cur /= d;
    }
}
