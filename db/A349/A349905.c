/*
 * A349905  Arithmetic derivative of A003961(n), where A003961 is fully
 *          multiplicative with a(p) = nextprime(p).
 * https://oeis.org/A349905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = arithmetic_derivative(A003961(n))
 * Range:     u8 n <= 31, u16 n <= 767, u32 n <= 9748, u64 n <= 9748,
 *            u128 n <= 9748, big any n
 * Helpers:   factor_u64, next_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003961 is fully multiplicative with a(p) = nextprime(p). */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* For n = 1, A003961(1) = 1 and its arithmetic derivative is 0. */
    if (n <= 1)
        return 0;

    /* Factor n: primes[i]^exps[i] */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute m = A003961(n) = ∏ nextprime(p)^e */
    seq_t m = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = next_prime(primes[i]);        /* next prime after p */
        m *= pow_uint(q, (idx_t)exps[i]);          /* q^e */
    }

    /* Compute the arithmetic derivative: sum e * (m / q) over all primes p|n. */
    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = next_prime(primes[i]);       /* same q as before */
        seq_t term = m / q;                      /* m/q is integer */
        term *= (seq_t)exps[i];
        result += term;
    }

    return result;
}
