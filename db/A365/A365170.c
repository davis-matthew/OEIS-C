/*
 * A365170  The sum of divisors d of n such that gcd(d, n/d) is squarefree.
 * https://oeis.org/A365170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sum_{d|n, gcd(d, n/d) squarefree} d. Multiplicative with
 *            a(p^e) = 1+p for e=1, a(p^2) = 1+p+p^2, a(p^e) =
 *            (1+p)*(1+p^(e-1)) for e>=3
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
    if (n == 1)
        return (seq_t)1;

    /* factorize n into primes and exponents */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t local;

        if (e == 1) {
            local = (seq_t)(1 + p);
        } else if (e == 2) {
            seq_t p2 = (seq_t)p * (seq_t)p;
            local = (seq_t)(1 + p + p2);
        } else { /* e >= 3 */
            seq_t p_pow = 1;
            for (uint64_t t = 0; t < e - 1; ++t)
                p_pow *= (seq_t)p;
            local = (seq_t)(1 + p) * (seq_t)(1 + p_pow);
        }

        result *= local;
    }

    return result;
}
