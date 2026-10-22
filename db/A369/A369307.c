/*
 * A369307  The number of exponentially odd divisors d of n such that n/d is
 *          also exponentially odd.
 * https://oeis.org/A369307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e in n of * a(p^e) = 2 if e
 *            is odd, else e/2 (for e even) * Thus a(1)=1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n) {
    /* For n < 1, return 0 (offset starts at 1). */
    if (n < 1) {
        return 0;
    }

    uint64_t m = (uint64_t)n;          /* work on native 64-bit integer */
    seq_t result = 1;                  /* multiplicative identity */

    while (m > 1) {
        uint64_t spf = _smallest_prime_factor(m);
        if (spf == 0) {                /* should not happen for m > 1 */
            break;
        }
        uint64_t exp = 0;
        while (m % spf == 0) {
            m /= spf;
            ++exp;
        }
        seq_t contrib = (seq_t)((exp & 1ULL) ? 2 : (exp / 2));
        result *= contrib;
    }

    return result;
}
