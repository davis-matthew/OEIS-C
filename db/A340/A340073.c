/*
 * A340073  a(n) = (x-1) / gcd(x-1, phi(x)), where x = A003961(n), i.e., n
 *          with its prime factorization shifted one step towards larger
 *          primes.
 * https://oeis.org/A340073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (x-1)/gcd(x-1,phi(x)) where x is n with each prime factor
 *            replaced by its successor prime
 * Range:     u8 n <= 47, u16 n <= 2047, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   calc_phi, factor_u64, gcd, next_prime, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/pow_int.h"
#include "../utils/gcd.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* Handle n >= 1. For n = 0 the value is undefined; return 0. */
    if (n == 0) return (seq_t)0;

    /* Factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the shifted product x */
    seq_t x = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t q = next_prime(p);          /* successor prime of p */
        seq_t pow = pow_int((seq_t)q, (idx_t)e);
        x *= pow;
    }

    /* Compute phi(x) and then (x-1)/gcd(x-1,phi(x)) */
    seq_t phi_x = calc_phi(x);
    seq_t x_minus1 = x - (seq_t)1;
    seq_t g = gcd(x_minus1, phi_x);
    return x_minus1 / g;
}
