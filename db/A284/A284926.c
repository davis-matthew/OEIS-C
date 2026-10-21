/*
 * A284926  a(n) = Sum_{d|n} (-1)^(n/d+1)*d^5.
 * https://oeis.org/A284926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d+1)*d^5
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 7130,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */

    /* factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t part;

        if (p == 2) {
            /* a(2^e) = (15*2^(5e+1)+1)/31  – use 128‑bit for safety */
            unsigned __int128 pow2 = (unsigned __int128)1 << (5 * e + 1);
            unsigned __int128 num   = pow2 * 15 + 1;
            unsigned __int128 val   = num / 31;
            part = (seq_t)val;
        } else {
            /* a(p^e) = 1 + p^5 + p^10 + … + p^(5e) */
            seq_t p5  = pow_uint(p, 5);
            seq_t term = 1;
            seq_t sum  = 1;
            for (uint64_t k = 1; k <= e; ++k) {
                term = term * p5;
                sum  = sum  + term;
            }
            part = sum;
        }

        result *= part;
    }

    return result;
}
