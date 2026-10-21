/*
 * A290480  Product of proper unitary divisors of n.
 * https://oeis.org/A290480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(2^(omega(n)-1)-1) (n ≥ 1), omega(n)=number of distinct
 *            primes dividing n For n = 1 the value is defined as 1. The
 *            exponent is 0 when n is a prime power, giving a(n)=1. The
 *            implementation counts distinct factors by calling factor_u64(),
 *            which is supplied in the utilities
 * Range:     u8 n <= 29, u16 n <= 41, u32 n <= 209, u64 n <= 569,
 *            u128 n <= 2309, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"   /* for factor_u64() */
#include "../utils/seq_pow.h"     /* for seq_pow() */

seq_t a(idx_t n)
{
    /* a(1) is defined as 1 */
    if (n == 1)
        return (seq_t)1;

    /* Count distinct prime factors of n. */
    uint64_t primes[64];        /* enough for any 64‑bit number */
    uint64_t exps[64];
    size_t   cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* omega = cnt distinct primes */
    idx_t omega = (idx_t)cnt;

    /* exponent = 2^(omega-1) - 1.  For omega>=1 this is safe
       because omega ≤ 64 for 64‑bit numbers. */
    idx_t exp = ((idx_t)1 << (omega - 1)) - 1;

    /* Return n^exp.  For exp == 0 seq_pow returns 1. */
    return seq_pow((seq_t)n, exp);
}
