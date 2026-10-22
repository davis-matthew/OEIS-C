/*
 * A334019  Sum of unitary divisors of n that are smaller than sqrt(n).
 * https://oeis.org/A334019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of unitary divisors d of n that are < sqrt(n)
 * Range:     u8 n <= 523, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* integer exponentiation – safe because p^e ≤ n ≤ 2^64-1 */
static uint64_t pow_uint_u64(uint64_t base, uint64_t exp)
{
    uint64_t r = 1;
    while (exp--)
        r *= base;
    return r;
}

seq_t a(idx_t n)
{
    if (n <= 1)   /* a(1) = 0, a(0) undefined but never called */
        return 0;

    /* factor n into primes and exponents */
    uint64_t primes[16];
    uint64_t exps  [16];
    size_t    cnt  = 0;
    factor_u64(n, primes, exps, &cnt);

    /* compute each prime power p^e once */
    uint64_t prime_pow[16];
    for (size_t i = 0; i < cnt; ++i)
        prime_pow[i] = pow_uint_u64(primes[i], exps[i]);

    seq_t sum = 0;
    size_t total_masks = ((size_t)1) << cnt;   /* cnt ≤ 16 for n < 2^64 */
    for (size_t mask = 0; mask < total_masks; ++mask)
    {
        idx_t d = 1;
        for (size_t j = 0; j < cnt; ++j)
            if (mask & ((size_t)1 << j))
                d *= prime_pow[j];

        /* test d < sqrt(n)  ⇔  d*d < n   (using seq_t to avoid overflow) */
        if ((seq_t)d * (seq_t)d < (seq_t)n)
            sum += (seq_t)d;
    }
    return sum;
}
