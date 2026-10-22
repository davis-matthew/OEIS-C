/*
 * A373369  a(n) = gcd(A001414(n), A059975(n)), where A001414 and A059975 are
 *          fully additive with a(p) = p and a(p) = p-1, respectively.
 * https://oeis.org/A373369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A001414(n), A059975(n)) where A001414(n) = Σ k·p
 *            (over prime powers p^k || n) A059975(n) = Σ k·(p-1) (over prime
 *            powers p^k || n)
 * Range:     u8 n <= 9908, u16 n <= 9908, u32 n <= 9908, u64 n <= 9908,
 *            u128 n <= 9908, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Compute the n-th term of OEIS A373369 */
seq_t a(idx_t n)
{
    /* Base case: a(1) = 0 */
    if (n <= 1)
        return 0;

    uint64_t m = (uint64_t)n;
    seq_t sum1 = 0;   /* Σ k·p   */
    seq_t sum2 = 0;   /* Σ k·(p-1) */

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t k = 0;
            while (m % p == 0) {
                m /= p;
                ++k;
            }
            sum1 += (seq_t)k * (seq_t)p;
            sum2 += (seq_t)k * ((seq_t)p - 1);
        }
    }

    if (m > 1) {               /* remaining prime factor */
        sum1 += (seq_t)m;
        sum2 += (seq_t)(m - 1);
    }

    if (sum1 == 0)              /* handles n = 1, though already caught */
        return 0;

    return gcd(sum1, sum2);
}
