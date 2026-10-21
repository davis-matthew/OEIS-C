/*
 * A272476  a(n) = n if n is prime, a(n) = 2*n+3 otherwise.
 * https://oeis.org/A272476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is prime, else a(n) = 2*n+3
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit integers.
   Works quickly for n up to at least 100 000 000. */
static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    uint64_t ul = (uint64_t)n;
    seq_t sn = (seq_t)ul;
    if (is_prime_u64(ul))
        return sn;
    return sn * (seq_t)2 + (seq_t)3;
}
