/*
 * A366905  The largest exponentially odious divisor of n.
 * https://oeis.org/A366905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e || n of p^f, where f = max{
 *            k : 1 <= k <= e and k has an odd number of 1-bits }. This is the
 *            largest exponentially odious divisor of n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_odious_exponent, pow_p
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/largest_odious_exponent.h"
#include "../utils/pow_p.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1, return 0 */

    uint64_t m = (uint64_t)n;

    seq_t result = 1;

    /* Handle factor 2 separately for speed */
    if (m % 2 == 0) {
        uint64_t e = 0;
        while (m % 2 == 0) {
            ++e;
            m /= 2;
        }
        uint64_t f = largest_odious_exponent(e);
        if (f)
            result *= pow_p(2, f);
    }

    /* Trial division by odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            uint64_t f = largest_odious_exponent(e);
            if (f)
                result *= pow_p(p, f);
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1) {
        /* exponent is 1, which is odious (popcount 1 = 1 odd) */
        result *= (seq_t)m;
    }

    return result;
}
