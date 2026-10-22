/*
 * A385006  The sum of the biquadratefree divisors of n.
 * https://oeis.org/A385006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e∥n} (p^min(e+1,4)-1)/(p-1)
 * Range:     u8 n <= 16, u16 n <= 2025, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* factorize n */
    uint64_t n64 = (uint64_t)n;
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64(n64, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t k = e + 1;
        if (k > 4) k = 4;

        seq_t pow_val = 1;
        for (uint64_t j = 0; j < k; ++j)
            pow_val *= (seq_t)p;

        seq_t term = (pow_val - 1) / (seq_t)(p - 1);
        result *= term;
    }
    return result;
}
