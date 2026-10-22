/*
 * A318465  The number of Zeckendorf-infinitary divisors of n = Product_{i}
 *          p(i)^r(i): divisors d = Product_{i} p(i)^s(i), such that the
 *          Zeckendorf expansion (A014417) of each s(i) contains only terms
 *          that are in the Zeckendorf expansion of r(i).
 * https://oeis.org/A318465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 2^{sum over prime–power exponents e of the number of
 *            terms in the Zeckendorf representation of e}. See OEIS A318465
 * Range:     u8 n <= 9899, u16 n <= 9899, u32 n <= 9899, u64 n <= 9899,
 *            u128 n <= 9899, big any n
 * Helpers:   factor_u64, pow2, zeckendorf_term_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/pow2.h"        /* pow2(idx_t exp) */
#include "../utils/zeckendorf_term_count.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for offsets <1, return 0 for safety */

    /* Factor n into prime powers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t    cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute exponent sum of 2 using Zeckendorf term counts of each exponent */
    int total_terms = 0;
    for (size_t i = 0; i < cnt; ++i) {
        total_terms += zeckendorf_term_count(exps[i]);
    }

    /* a(n) = 2^{total_terms} */
    return pow2((idx_t)total_terms);
}
