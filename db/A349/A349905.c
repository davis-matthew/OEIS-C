/*
 * A349905  Arithmetic derivative of A003961(n), where A003961 is fully
 *          multiplicative with a(p) = nextprime(p).
 * https://oeis.org/A349905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = arithmetic_derivative(A003961(n))
 * Range:     u8 n <= 31, u16 n <= 767, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 96 terms through n = 9748 (2026-11-01)
 * Helpers:   factor_u64, ipow_u64_idx, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003961 is fully multiplicative with a(p) = nextprime(p). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"
#include "../utils/next_prime.h"

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
        m *= ipow_u64_idx(q, (idx_t)exps[i]);          /* q^e */
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
