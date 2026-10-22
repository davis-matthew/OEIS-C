/*
 * A351569  Sum of divisors of the largest unitary divisor of n that is an
 *          exponentially odd number.
 * https://oeis.org/A351569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   multiplicative: a(p^e) = (p^(e+1)-1)/(p-1) if e odd, else 1
 * Range:     u8 n <= 16, u16 n <= 5929, u32 n <= 9791, u64 n <= 9791,
 *            u128 n <= 9791, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* protection for n < 1 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if (e & 1ULL) { /* odd exponent */
            seq_t num = seq_pow((seq_t)p, (idx_t)(e + 1)) - 1;
            seq_t den = (seq_t)(p - 1);
            seq_t sigma = num / den;
            result *= sigma;
        }
    }
    return result;
}
