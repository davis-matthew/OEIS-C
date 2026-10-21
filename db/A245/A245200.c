/*
 * A245200  Smallest positive solution to k == 0 mod 3 and k == 1 mod
 *          prime(n).
 * https://oeis.org/A245200
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * inverse_mod(3, prime(n)) * i.e. the smallest positive
 *            k such that k ≡ 0 (mod 3) and k ≡ 1 (mod prime(n))
 * Range:     u8 n <= 33, u16 n <= 3513, u32 n <= 9753, u64 n <= 9753,
 *            u128 n <= 9753, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* For n >= 3, prime(n) >= 5, so gcd(3, prime(n)) == 1. */
    uint32_t p = get_prime(n);                             /* prime(n) */
    seq_t inv3 = mod_pow((seq_t)3, (idx_t)p - 2, (seq_t)p); /* 3^(p-2) mod p */
    return 3 * inv3;                                        /* 3 * inverse of 3 modulo p */
}
