/*
 * A245447  Permutation of natural numbers: a(n) = A048673(A048673(n)).
 * https://oeis.org/A245447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 659, u32 n <= 9659, u64 n <= 9659,
 *            u128 n <= 9659, big any n
 * Helpers:   factor_u64, next_prime, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A245447 – Permutation of natural numbers
 * Formula: a(n) = (1/2) * (1 + A003961((1/2) * (1 + A003961(n))))
 * where A003961(n) shifts each prime factor p^e of n to the next prime
 * q^e (q = next prime after p).  The shift uses the helper
 * next_prime from the utils library.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_int.h"
#include "../utils/next_prime.h"

/* Compute A003961: shift prime powers of n to next prime. */
static seq_t A003961(seq_t value)
{
    if (value < 2)        /* 0 or 1 stay unchanged */
        return value;

    /* factor(value) – value fits in uint64_t for all admissible n. */
    uint64_t n = (uint64_t)value;
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    factor_u64(n, primes, exps, &pcnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t next = next_prime(primes[i]);               /* prime after primes[i] */
        seq_t term    = pow_int((seq_t)next, (idx_t)exps[i]); /* next^exponent */
        result *= term;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t inner   = A003961((seq_t)n);
    seq_t half    = (inner + (seq_t)1) / (seq_t)2;
    seq_t outer   = A003961(half);
    seq_t result  = (outer + (seq_t)1) / (seq_t)2;
    return result;
}
