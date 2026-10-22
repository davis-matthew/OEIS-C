/*
 * A385379  The maximum possible number of distinct composite prime powers
 *          (A246547) in the factorization of n into prime powers.
 * https://oeis.org/A385379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum over prime factors p^e of f(e), where f(e) is the
 *            maximum number of distinct composite prime powers (exponents >=
 *            2) that sum to e. f(e) can be computed by a greedy algorithm:
 *            keep adding the smallest possible exponent (starting at 2) as
 *            long as the running sum does not exceed e
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   max_distinct_comp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/max_distinct_comp.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* a(1) = 0, and a(0) is irrelevant */
    uint64_t t = (uint64_t)n;
    seq_t result = 0;

    /* handle factor 2 */
    int e = 0;
    while ((t & 1ULL) == 0) {
        t >>= 1;
        e++;
    }
    if (e >= 2) result += max_distinct_comp(e);

    /* odd factors */
    uint64_t p = 3;
    while (p * p <= t) {
        if (t % p == 0) {
            e = 0;
            while (t % p == 0) {
                t /= p;
                e++;
            }
            if (e >= 2) result += max_distinct_comp(e);
        }
        p += 2;
    }
    if (t > 1) { /* remaining prime factor with exponent 1 */
        /* exponent is 1 < 2 so contributes 0 */
    }
    return result;
}
