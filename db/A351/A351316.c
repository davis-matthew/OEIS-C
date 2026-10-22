/*
 * A351316  Sum of the 10th powers of the square divisors of n.
 * https://oeis.org/A351316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d^2|n} (d^2)^10
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 15, u64 n <= 99, u128 n <= 7224,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Factor n into primes */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t t = e / 2;          /* floor(e/2) */

        if (t == 0)
            continue;                /* contribution is 1 */

        /* Compute p^20 using 128‑bit intermediate to avoid overflow */
        __uint128_t pow20_128 = 1;
        for (int k = 0; k < 20; ++k)
            pow20_128 *= (unsigned long long)p;
        seq_t pow20 = (seq_t)pow20_128;   /* truncated if too large */

        seq_t sum  = 1;
        seq_t term = 1;
        for (uint64_t k = 1; k <= t; ++k) {
            term = term * pow20;   /* may overflow for 64‑bit builds */
            sum  = sum  + term;
        }

        result = result * sum;
    }

    return result;
}
