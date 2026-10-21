/*
 * A246489  Duodecimal period of 1/(n-th prime) (0 by convention for the
 *          primes 2 and 3).
 * https://oeis.org/A246489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = multiplicative order of 12 modulo the n-th prime, with 0
 *            returned for the primes 2 and 3 (OEIS convention)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/factorize.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    /* retrieve the n-th prime; the function returns it as a 32‑bit unsigned integer */
    uint64_t prime = (uint64_t)get_prime(n);

    /* by definition a(1)=0 for prime 2, and a(2)=0 for prime 3 */
    if (prime <= 3) {
        return (seq_t)0;
    }

    /* the multiplicative order of 12 modulo prime */
    uint64_t order = prime - 1;          /* φ(prime) = prime-1 */
    uint64_t prime_factors[64];
    int exp_factors[64];
    size_t pf_cnt = factorize(prime - 1, prime_factors, exp_factors);

    /* reduce the order by each prime factor in the factorisation of (prime-1) */
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t q = prime_factors[i];
        while (order % q == 0 &&
               mod_pow((seq_t)12, order / q, (seq_t)prime) == (seq_t)1) {
            order /= q;
        }
    }

    return (seq_t)order;
}
