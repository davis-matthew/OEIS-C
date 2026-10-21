/*
 * A258647  Seventh arithmetic derivative of n.
 * https://oeis.org/A258647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (arithmetical_derivative(n))⁷ Compute the seventh
 *            iteration of the arithmetic derivative of the integer n
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 7167, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Compute the arithmetic derivative of a 64‑bit integer.
   Returns the result as a seq_t. */
static seq_t arithmetic_derivative_u64(uint64_t n)
{
    if (n < 2)
        return 0;

    uint64_t primes[64];     /* prime factors          */
    uint64_t exps[64];       /* corresponding exponents*/
    size_t   cnt;            /* number of distinct factors */

    factor_u64(n, primes, exps, &cnt);

    seq_t nval = (seq_t)n;   /* working copy as seq_t      */
    seq_t result = 0;        /* accumulated sum            */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (nval / (seq_t)primes[i]) * (seq_t)exps[i];
        result += term;
    }
    return result;
}

/* Public function: seventh arithmetic derivative of n. */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;          /* start from n */

    for (int i = 0; i < 7; ++i) {  /* iterate 7 times   */
        val = arithmetic_derivative_u64((uint64_t)val);
    }
    return val;
}
