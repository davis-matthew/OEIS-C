/*
 * A296931  Primes p such that Legendre(-17,p) = 0 or 1.
 * https://oeis.org/A296931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 3266, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that Legendre(-17, p) = 0 or 1.
   The function finds the n‑th such prime by searching primes
   sequentially and testing the Jacobi symbol. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/next_prime.h"

static inline bool good_prime(uint64_t p)
{
    /* Legendre(-17, p) equals Jacobi(-17 mod p, p).
       For p = 17 it is 0, which is accepted.         */
    if (p == 17u) {
        return true;
    }

    uint64_t a = (p > 17u) ? (p - 17u) : (p - (17u % p));
    return jacobi_symbol(a, p) == 1;
}

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)0; /* undefined for 0, but harmless */
    }

    idx_t count = 0u;
    uint64_t p = 1u; /* will be replaced by the first prime (2) */

    while (count < n) {
        p = next_prime(p);

        if (good_prime(p)) {
            ++count;
        }
    }

    return (seq_t)p;
}
