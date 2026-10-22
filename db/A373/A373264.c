/*
 * A373264  a(n) = 1 if n' / gcd(n,n') is of the form 4m+2, otherwise 0, where
 *          n' stands for the arithmetic derivative of n, A003415.
 * https://oeis.org/A373264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1036, u16 n <= 9368, u32 n <= 9368, u64 n <= 9368,
 *            u128 n <= 9368, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1 if n' / gcd(n,n') is congruent to 2 (mod 4), else 0.
 * Here n' is the arithmetic derivative of n.
 */

#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

/* Compute the arithmetic derivative of n (n >= 1). */
static seq_t arithmetic_derivative(idx_t n)
{
    /* For n <= 1 the derivative is 0. */
    if (n <= 1)
        return 0;

    /* Factorization: primes[i]^(exponents[i]), count = cnt. */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t cnt = 0;
    factor_u64(n, primes, exponents, &cnt);

    seq_t n_seq = (seq_t)n;
    seq_t d = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        /* term = e * (n / p) */
        seq_t term = (seq_t)exponents[i] * (n_seq / (seq_t)primes[i]);
        d += term;
    }
    return d;
}

/* OEIS sequence A373264. */
seq_t a(idx_t n)
{
    seq_t d = arithmetic_derivative(n);          /* n'          */
    seq_t g = gcd(d, (seq_t)n);                 /* gcd(n,n')   */
    seq_t q = d / g;                            /* n'/gcd(...) */
    seq_t rem = q & 3;                          /* q mod 4     */
    return (rem == 2) ? 1 : 0;
}
