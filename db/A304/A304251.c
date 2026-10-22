/*
 * A304251  If n = Product (p_j^k_j) then a(n) = Sum (prime(p_j)^k_j).
 * https://oeis.org/A304251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p^k‖n} prime(p)^k (sum over prime powers in the
 *            factorization of n)
 * Range:     u8 n <= 48, u16 n <= 2047, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n == (idx_t)1) {
        return (seq_t)0;
    }

    /* Arrays big enough for any n up to the limits typical for the problem. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = (seq_t)0;
    for (size_t i = (size_t)0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        idx_t   e    = (idx_t)exps[i];          /* exponent */
        uint32_t prime_of_p = get_prime(p);     /* p-th prime */
        seq_t term = pow_uint((uint64_t)prime_of_p, e);
        sum += term;
    }
    return sum;
}
