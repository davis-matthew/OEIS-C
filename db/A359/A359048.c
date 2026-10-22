/*
 * A359048  a(n) is the minimum denominator d such that the decimal expansion
 *          of n/d is eventually periodic with periodicity not equal to zero.
 * https://oeis.org/A359048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, hear, nonn
 * Formula:   a(n) = the smallest prime power p^e (p prime, p∤10, e≥1) that
 *            does not divide n
 * Range:     u8 n <= 9645, u16 n <= 9645, u32 n <= 9645, u64 n <= 9645,
 *            u128 n <= 9645, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    seq_t best = 0;                  /* 0 means unset */

    for (idx_t p = 3; ; ++p) {
        if (p == 2 || p == 5) continue;          /* primes that divide 10 */
        if (!is_prime(p)) continue;              /* keep to primes only */

        /* If we already have a candidate that is smaller than the current p,
           no later prime can beat it because every higher prime power is ≥ p.  */
        if (best != 0 && (seq_t)p > best) break;

        /* Count multiplicity of p in n. */
        idx_t exp = 0;
        idx_t tmp = n;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }

        /* Candidate is p^(exp+1).  Compute multiplicatively. */
        seq_t cand = 1;
        for (idx_t i = 0; i <= exp; ++i)
            cand *= (seq_t)p;

        if (best == 0 || cand < best) best = cand;

        /* 3 is the theoretical lower bound (smallest prime not dividing 10). */
        if (best == 3) break;
    }

    return best;
}
