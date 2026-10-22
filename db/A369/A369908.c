/*
 * A369908  a(n) = Sum_{p|n, p prime} n^Omega(n/p).
 * https://oeis.org/A369908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{p|n, p prime} n^Ω(n/p)
 * Range:     u8 n <= 11, u16 n <= 31, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by observing that Ω(n/p) = Ω(n) - 1
   for every distinct prime p dividing n.  Let cnt be the number
   of distinct prime divisors and tot be Ω(n); then

        a(n) = cnt * n^(tot-1)    (for n > 1)

   The remaining work is to factor n and sum its exponents.
*/

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */
#include "../utils/big_pow.h"      /* big_pow(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 0 */
    if (n <= 1) return (seq_t)0;

    /* Factor n into prime powers.  A buffer of 64 entries is
       more than enough for n <= 2^64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Total number of prime factors (with multiplicity). */
    size_t tot = 0;
    for (size_t i = 0; i < cnt; ++i) tot += exps[i];

    /* Compute n^(tot-1).  The exponent is always >= 0 because
       n>1 guarantees tot≥1. */
    seq_t base  = (seq_t)n;
    seq_t pow   = big_pow(base, (seq_t)(tot - 1));

    /* Multiply by the number of distinct prime divisors. */
    seq_t result = pow * (seq_t)cnt;
    return result;
}
