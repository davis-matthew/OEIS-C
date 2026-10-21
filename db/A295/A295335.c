/*
 * A295335  a(n) = least k >= 0 such that n OR k is prime (where OR denotes
 *          the bitwise OR operator).
 * https://oeis.org/A295335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 243, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined as a(n) = least k ≥ 0 such that (n OR k) is prime.
   For example, a(42) = 1 because 42|0 = 42 (not prime), 42|1 = 43 (prime). */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t k = 0;
    for (;; ++k) {
        seq_t cand = (seq_t)n | (seq_t)k;
        if (is_prime_big(cand))
            return (seq_t)k;
    }
}
