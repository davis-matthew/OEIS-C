/*
 * A375933  The second-largest exponent in the prime factorization of n, or 0
 *          if it does not exist.
 * https://oeis.org/A375933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = second largest *distinct* exponent in the prime
 *            factorization of n; 0 if it does not exist
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"  /* provides factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

seq_t a(idx_t n)
{
    /* maximum number of distinct prime factors of a 64‑bit integer is < 64 */
    static const size_t MAX_FACTORS = 64;
    uint64_t pr[MAX_FACTORS];
    uint64_t ex[MAX_FACTORS];
    size_t   cnt = 0;

    if (n <= 1)
        return 0;

    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* If |exponents| < 2, no second largest exponent. */
    if (cnt < 2)
        return 0;

    /* Track the largest and the second largest distinct exponents. */
    uint64_t max1 = 0;  /* largest exponent */
    uint64_t max2 = 0;  /* second largest distinct exponent */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = ex[i];
        if (e > max1) {
            max2 = max1;
            max1 = e;
        } else if (e < max1 && e > max2) {
            /* e is distinct from max1 and larger than current second largest */
            max2 = e;
        }
    }

    return (seq_t)max2;  /* max2 is 0 if no second distinct exponent exists */
}
