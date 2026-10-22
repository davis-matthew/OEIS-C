/*
 * A340673  If n is of the form s^(2^e), where s is a squarefree number, and e
 *          >= 0, then a(n) is the (1+e)-th prime, otherwise a(n) = 1.
 * https://oeis.org/A340673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9757, u16 n <= 9757, u32 n <= 9757, u64 n <= 9757,
 *            u128 n <= 9757, big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A340673:
   a(n) = (1+e)-th prime if n = s^(2^e) with s squarefree and e ≥ 0,
   otherwise a(n) = 1.  a(1) = 1. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/get_prime.h"   /* get_prime */

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 */
    if (n == 1)
        return (seq_t)1;

    /* Factor n.  We need at most 64 distinct prime factors for 64‑bit n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* If n has no prime factors (should not happen for n>1), treat as 1. */
    if (cnt == 0)
        return (seq_t)1;

    /* All exponents must be equal. */
    uint64_t exp = exps[0];
    for (size_t i = 1; i < cnt; ++i)
    {
        if (exps[i] != exp)
            return (seq_t)1;
    }

    /* The common exponent must be a power of two. */
    if (exp == 0 || (exp & (exp - 1)) != 0)
        return (seq_t)1;

    /* Compute e such that exp = 2^e. */
    unsigned int e = 0;
    uint64_t tmp = exp;
    while (tmp > 1)
    {
        tmp >>= 1;
        ++e;
    }

    /* Return the (1+e)-th prime. */
    uint32_t p = get_prime((idx_t)(e + 1));
    return (seq_t)p;
}
