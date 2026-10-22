/*
 * A349382  Dirichlet convolution of A064989 with A346234 (Dirichlet inverse
 *          of A003961), where A003961 and A064989 are fully multiplicative
 *          sequences that shift the prime factorization of n one step towards
 *          larger and smaller primes respectively.
 * https://oeis.org/A349382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = ∏_{p^e‖n} f(p,e) where f(2,e) = -2, f(p,e) =
 *            prevprime(p)^e – nextprime(p)·prevprime(p)^(e−1) (p>2)
 * Range:     i8 n <= 209, i16 n <= 9550, i32 n <= 9550, i64 n <= 9550,
 *            i128 n <= 9550, big any n
 * Helpers:   factor_u64, next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helpers below are provided by the build environment. */
#include <stdint.h>
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/prev_prime.h"  /* idx_t prev_prime(idx_t n); */
#include "../utils/next_prime.h"  /* idx_t next_prime(idx_t n); */

/* Compute base^exp where base is seq_t and exp is idx_t */
static seq_t power_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

/* Return the term contributed by the prime power p^e */
static seq_t prime_power_term(idx_t p, idx_t e)
{
    if (p == (idx_t)2)
        return (seq_t)-2;                      /* a(2^e) = -2 */

    idx_t prev_p = prev_prime(p);
    idx_t next_p = next_prime(p);

    seq_t prev_pow_e   = power_seq((seq_t)prev_p, e);
    seq_t prev_pow_ep1 = power_seq((seq_t)prev_p, e - 1);

    return prev_pow_e - (seq_t)next_p * prev_pow_ep1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Allocate temporary arrays large enough for all prime factors of n */
    /* n ≤ 10 000, so at most 5 prime factors (2·3·5·7·11·13 > 10 000) */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
        result *= prime_power_term((idx_t)primes[i], (idx_t)exps[i]);

    return result;
}
