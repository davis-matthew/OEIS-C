/*
 * A342414  a(n) = A003415(n) / gcd(phi(n),A003415(n)), where A003415(n) is
 *          the arithmetic derivative of n, and phi is Euler totient function.
 * https://oeis.org/A342414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(n) / gcd(phi(n), A003415(n))
 * Range:     u8 n <= 95, u16 n <= 9286, u32 n <= 9286, u64 n <= 9286,
 *            u128 n <= 9286, big any n
 * Helpers:   factor_u64, gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/phi.h"

/* Arithmetic derivative of n: A003415(n) */
static seq_t arithmetic_derivative(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t n_seq = (seq_t)n;
    seq_t deriv = 0;

    /* factorization of n: primes[i]^exps[i] */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        idx_t    e = (idx_t)exps[i];

        /* term = n * e / p  (all integer operations) */
        seq_t term = n_seq * (seq_t)e / (seq_t)p;
        deriv += term;
    }
    return deriv;
}

/* OEIS A342414 */
seq_t a(idx_t n)
{
    seq_t deriv = arithmetic_derivative(n);
    seq_t phi_n = phi(n);          /* Euler totient function */
    seq_t g     = gcd(deriv, phi_n); /* gcd */
    return deriv / g;
}
