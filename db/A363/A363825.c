/*
 * A363825  The number of infinitary divisors of n that are exponentially odd
 *          numbers (A268335).
 * https://oeis.org/A363825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e∥n} ( e even ? 1 : 2^{popcount(e-1)} + 1 )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* a(0) is undefined; offset starts at 1 */

    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    /* handle prime 2 */
    unsigned int e2 = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        e2++;
    }
    if (e2 > 0 && (e2 & 1)) {      /* odd exponent for 2 */
        unsigned int pop = __builtin_popcountll((unsigned long long)(e2 - 1));
        seq_t term = pow2(pop);
        term += (seq_t)1;
        result *= term;
    }

    /* handle odd primes */
    for (uint64_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) {
            unsigned int e = 0;
            while (m % d == 0) {
                m /= d;
                e++;
            }
            if (e & 1) {           /* odd exponent */
                unsigned int pop = __builtin_popcountll((unsigned long long)(e - 1));
                seq_t term = pow2(pop);
                term += (seq_t)1;
                result *= term;
            }
            /* even exponent contributes factor 1, nothing to do */
        }
    }

    /* remaining prime factor, exponent 1 (odd) */
    if (m > 1) {
        seq_t term = pow2(0);
        term += (seq_t)1;
        result *= term;
    }

    return result;
}
