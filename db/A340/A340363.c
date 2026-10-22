/*
 * A340363  a(n) = 1 if n is of the form of 2^i * p^j, with p an odd prime and
 *          i, j >= 0, otherwise 0.
 * https://oeis.org/A340363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is of the form 2^i · p^j with p an odd prime
 *            (i,j≥0), else 0
 * Range:     u8 n <= 9753, u16 n <= 9753, u32 n <= 9753, u64 n <= 9753,
 *            u128 n <= 9753, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* OEIS offset 1: a(1) is the first term.  Safe‑guard for n==0. */
    if (n == 0)
        return 0;

    /* Strip out all factors of 2 to obtain the odd part of n. */
    idx_t odd = n;
    while ((odd & 1u) == 0)
        odd >>= 1;

    /* n is a power of two (odd part = 1).  By definition the term is 1. */
    if (odd == 1)
        return 1;

    /* Factor the odd part and check the number of distinct prime factors. */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64((uint64_t)odd, primes, exps, &cnt);

    return (seq_t)(cnt <= 1 ? 1 : 0);
}
