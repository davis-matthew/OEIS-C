/*
 * A343249  a(n) is the least k0 <= n such that v_2(n), the 2-adic order of n,
 *          can be obtained by the formula: v_2(n) = log_2(n / L_2(k0, n)),
 *          where L_2(k0, n) is the lowest common denominator of the elements
 *          of the set S_2(k0, n) = {(1/n)*binomial(n, k), with 0 < k <= k0
 *          such that k is not divisible by 2} o...
 * https://oeis.org/A343249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Conjecture: a(n) = largest odd prime power divisor of n */

/* The implementation uses the helper function factor_u64
   declared in the build utils. */
#include "../utils/factor_u64.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial cases first. */
    if (n <= 1)               /* n == 1 */
        return (seq_t)1;

    /* Remove all factors of 2.  After this, if the remainder is 1,
       the original n was a power of two. */
    uint64_t odd = n;
    while ((odd & 1ULL) == 0)
        odd >>= 1;
    if (odd <= 1)
        return (seq_t)1;      /* power of 2, return 1 */

    /* Factor the odd part.  The maximal number of distinct primes
       for a 64‑bit integer is far below 20, so a small static buffer
       suffices. */
    const size_t MAX_FACTORS = 20;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;
    factor_u64(odd, primes, exps, &cnt);

    /* Find the largest odd prime power. */
    seq_t best = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t power = (seq_t)1;
        for (uint64_t e = 0; e < exps[i]; ++e)
            power *= (seq_t)primes[i];
        if (power > best)
            best = power;
    }

    return best;
}
