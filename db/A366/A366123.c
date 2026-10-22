/*
 * A366123  The number of prime factors of the cube root of the largest cube
 *          dividing n, counted with multiplicity.
 * https://oeis.org/A366123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{p|n} ⌊v_p(n)/3⌋
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A366123: number of prime factors (with multiplicity) of the cube
 *  root of the largest cube dividing n.  Uses factor_u64 to factor n
 *  and then sums the integer division of each exponent by 3. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1U)
        return (seq_t)0;   /* a(1)=0, n=0 is outside the offset */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i)
        result += (seq_t)(exps[i] / 3U);

    return result;
}
