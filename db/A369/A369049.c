/*
 * A369049  a(n) = n mod n', where n' is the arithmetic derivative of n,
 *          A003415.
 * https://oeis.org/A369049
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n mod A003415(n) (arithmetic derivative of n)
 * Range:     u8 n <= 255, u16 n <= 9527, u32 n <= 9527, u64 n <= 9527,
 *            u128 n <= 9527, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Computes the arithmetic derivative A003415(n) and returns n mod derivative. */
seq_t a(idx_t n)
{
    /* n ≥ 2 for all valid calls.  If called with n < 2, behaviour is undefined. */
    seq_t sn = (seq_t)n;

    /* 64‑bit factorisation is enough for all n that fit in idx_t. */
    uint64_t primes[64], exps[64];
    size_t  cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t der = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t prime  = (seq_t)primes[i];
        seq_t exp    = (seq_t)exps[i];
        /* derivative of p^k is k * p^(k-1).  Since n is divisible by p, n/p is integer. */
        seq_t term = (sn / prime) * exp;
        der += term;
    }

    /* For n ≥ 2, the derivative is never zero, so modulo is safe. */
    return sn % der;
}
