/*
 * A254939  a(n) = (A099795(n)^-1 mod p)*A099795(n), where p = prime(n).
 * https://oeis.org/A254939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (L^-1 mod p) * L where L = lcm(1..p-1) and p = the n-th
 *            prime
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   get_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/powmod64.h"

seq_t a(idx_t n)
{
    /* Obtain the n-th prime. */
    uint32_t p = get_prime(n);

    /* Compute L = lcm(1..p-1) as product of prime powers. */
    seq_t L = 1;
    /* Iterate over all primes q <= p-1. */
    for (idx_t k = 1;; ++k) {
        uint32_t q = get_prime(k);
        if (q >= p) break;           /* stop when q reaches p */
        uint32_t limit = p - 1;
        uint64_t power = q;
        while (power * (uint64_t)q <= (uint64_t)limit) {
            power *= q;              /* find highest power of q <= limit */
        }
        L = L * (seq_t)power;        /* multiply into L */
    }

    /* Compute L modulo p. */
    uint64_t Lmodp = (uint64_t)(L % (seq_t)p);

    /* Compute modular inverse of Lmodp modulo p (p is prime). */
    uint64_t inv = powmod64(Lmodp, (uint64_t)p - 2, p);

    /* Result is L * inv. */
    seq_t result = L * (seq_t)inv;
    return result;
}
