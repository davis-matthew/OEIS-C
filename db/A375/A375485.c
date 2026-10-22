/*
 * A375485  a(n) is the number of integers k between 0 and n such that n XOR k
 *          is a prime number (where XOR denotes the bitwise XOR operator).
 * https://oeis.org/A375485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = #{ k | 0 ≤ k ≤ n , is_prime(n XOR k) }
 * Range:     u8 n <= 1749, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Count how many integers k in [0, n] produce a prime when XOR‑ed with n */
    idx_t cnt = 0;
    for (idx_t k = 0; k <= n; ++k) {
        if (is_prime(n ^ k)) {
            ++cnt;
        }
    }
    return (seq_t)cnt;
}
