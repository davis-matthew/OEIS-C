/*
 * A335302  a(n) = prime(n+1) mod (2^k) where k is the least positive integer
 *          such that floor(prime(n)/(2^k)) = floor(prime(n+1)/(2^k)) and
 *          prime(n) denotes the n-th prime number.
 * https://oeis.org/A335302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 6541, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A335302: a(n) = prime(n+1) mod 2^k where k is the least positive integer
 * such that floor(prime(n)/2^k) = floor(prime(n+1)/2^k).
 * (Equivalently, k is one more than the position of the highest differing
 * bit between consecutive primes.)
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* returns the k-th prime as uint32_t */

seq_t a(idx_t n)
{
    /* n is 1‑based, we need primes n and n+1 */
    seq_t p1 = (seq_t)get_prime((idx_t)n);      /* prime(n)   */
    seq_t p2 = (seq_t)get_prime((idx_t)n + 1);  /* prime(n+1) */

    /* find smallest k ≥ 1 with (p1 >> k) == (p2 >> k) */
    idx_t k = 1;
    while (((p1 >> k) != (p2 >> k))) {
        ++k;
    }

    /* mask for the lowest k bits of p2 */
    seq_t mask = ((seq_t)1 << k) - 1;
    return p2 & mask;
}
