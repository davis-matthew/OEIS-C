/*
 * A378606  Dirichlet convolution of A046692 and A003961, where A046692 is the
 *          Dirichlet inverse of sigma, and A003961 is fully multiplicative
 *          with a(prime(i)) = prime(i+1).
 * https://oeis.org/A378606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} A046692(d)*A003961(n/d)
 * Range:     u8 n <= 127, u16 n <= 4095, u32 n <= 8980, u64 n <= 8980,
 *            u128 n <= 8980, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative closure: for prime powers p^e
 * a(p)   = q(p) - p - 1
 * a(p^e) = q(p)^e - (p+1)*q(p)^(e-1) + p*q(p)^(e-2)   (e ≥ 2)
 * where q(p) is the next prime after p. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

static seq_t
next_prime_after(idx_t p)
{
    idx_t q = p + 1;
    while (!is_prime(q))
        ++q;
    return (seq_t)q;
}

static seq_t
prime_pow_term(uint64_t p, unsigned e)
{
    seq_t p_seq = (seq_t)p;
    seq_t q_seq = next_prime_after((idx_t)p);

    /* precompute powers of q up to e */
    seq_t pow_q[64];
    pow_q[0] = (seq_t)1;
    for (unsigned k = 1; k <= e; ++k)
        pow_q[k] = pow_q[k - 1] * q_seq;

    if (e == 1) {
        /* q - p - 1 */
        return q_seq - p_seq - (seq_t)1;
    } else {
        /* q^e - (p+1)*q^(e-1) + p*q^(e-2) */
        return pow_q[e]
             - (p_seq + (seq_t)1) * pow_q[e - 1]
             + p_seq * pow_q[e - 2];
    }
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 0)
        return (seq_t)0;   /* safety, unused */

    /* factor n */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
        result *= prime_pow_term(primes[i], (unsigned)exps[i]);

    return result;
}
