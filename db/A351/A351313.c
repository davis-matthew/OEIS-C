/*
 * A351313  Sum of the 7th powers of the square divisors of n.
 * https://oeis.org/A351313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d^2 | n} (d^2)^7 = Sum_{d | n, d is square} d^7
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 24, u64 n <= 575,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n = 0 in OEIS offset 1 */
    uint64_t nn = (uint64_t)n;

    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t t = e >> 1;              /* floor(e/2) */

        seq_t term = (seq_t)1;            /* holds 1 + p^14 + p^28 + ... */

        if (t > 0) {
            seq_t p14 = seq_pow((seq_t)p, (idx_t)14);
            seq_t cur = (seq_t)1;
            for (uint64_t k = 1; k <= t; ++k) {
                cur *= p14;
                term += cur;
            }
        }
        result *= term;
    }
    return result;
}
