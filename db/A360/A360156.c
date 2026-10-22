/*
 * A360156  a(n) is the sum of the even unitary divisors of 2*n.
 * https://oeis.org/A360156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(v2(n)+1) * ∏_{p^e||odd_part(n)} (1 + p^e)
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The product runs over all odd prime powers in the factorisation of the odd part of n. */
/* The function uses the helper factor_u64 to obtain the prime factors of an odd integer. */

#include <stdint.h>
#include <stddef.h>
/* Helper that partitions an odd integer into its prime powers. */
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Determine v2(n) and the odd part m. */
    idx_t k = 0;                /* exponent of 2 in n */
    uint64_t temp = (uint64_t)n;
    while ((temp & 1ULL) == 0ULL)  /* while divisible by 2 */
    {
        temp >>= 1;
        ++k;
    }
    uint64_t m = temp;          /* odd part of n */

    /* Compute prod = ∏_{p^e||m} (1 + p^e)  (unitary divisor sum of m). */
    seq_t prod = 1;

    /* Containers for prime factors and their exponents of m. */
    /* 64 slots more than enough for any 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p_pow = 1;
        for (uint64_t j = 0; j < exps[i]; ++j)
            p_pow = p_pow * (seq_t)primes[i];

        prod = prod * ((seq_t)1 + p_pow);
    }

    /* Compute 2^(k+1). */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < k + 1; ++i)
        pow2 = pow2 * (seq_t)2;

    return pow2 * prod;
}
