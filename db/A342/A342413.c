/*
 * A342413  a(n) = gcd(phi(n), A003415(n)), where A003415(n) is the arithmetic
 *          derivative of n, and phi is Euler totient function.
 * https://oeis.org/A342413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(phi(n), arithmetic_derivative(n)) for n ≥ 1.
 *            Arithmetic derivative is computed by prime factorization: d(n) =
 *            n * Σ (e_i / p_i) where n = Π p_i^e_i
 * Range:     u8 n <= 510, u16 n <= 8997, u32 n <= 8997, u64 n <= 8997,
 *            u128 n <= 8997, big any n
 * Helpers:   factor_u64, gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/phi.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* The arithmetic derivative of 1 is 0, so a(1)=gcd(φ(1),0)=1. */
    if (n == 1) {
        return 1;
    }

    /* Factor n: primes[] holds the distinct prime factors,
       exps[] holds their exponents.  */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the arithmetic derivative. */
    seq_t deriv = 0;
    seq_t n_seq = (seq_t)n;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)exps[i] * (n_seq / (seq_t)primes[i]);
        deriv += term;
    }

    /* Compute φ(n). */
    seq_t phi_val = phi(n);

    /* Return gcd(φ(n), d(n)). */
    return gcd(phi_val, deriv);
}
