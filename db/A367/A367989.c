/*
 * A367989  The sum of square divisors of the largest unitary divisor of n
 *          that is a square.
 * https://oeis.org/A367989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = multiplicative with a(p^e) = (p^(e+2)-1)/(p^2-1) if e is
 *            even, 1 otherwise
 * Range:     u8 n <= 224, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* offset is 1   */

    /* factorisation of n          */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t exp = exps[i];
        if ((exp & 1ULL) == 0) {          /* even exponent only */
            seq_t sum  = 1;                /* 1 + p^2 + p^4 + … + p^e  */
            seq_t step = (seq_t)primes[i] * (seq_t)primes[i]; /* p^2          */
            uint64_t half = exp >> 1;     /* number of p^2–steps  */

            for (uint64_t j = 1; j <= half; ++j) {
                sum += step;
                step *= (seq_t)primes[i] * (seq_t)primes[i]; /* next p^2 */
            }
            result *= sum;
        }
    }

    return result;
}
