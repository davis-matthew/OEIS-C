/*
 * A360720  a(n) is the sum of unitary divisors of n that are powerful
 *          (A001694).
 * https://oeis.org/A360720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n, e>1} (1 + p^e) (product over prime powers of n
 *            with exponent >1)
 * Range:     u8 n <= 224, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1; for n ≥ 2, a(n) = 1 if n has no prime factor with exponent >1, else the
   product above. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    if (m == 1) return (seq_t)1;           /* n = 1 has only the unitary divisor 1 */

    seq_t prod = (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(m, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = exps[i];
        if (e > 1)
        {
            /* Compute p^e in arbitrary‑precision type seq_t */
            seq_t p_pow = big_pow((seq_t)primes[i], (seq_t)e);
            prod *= (p_pow + (seq_t)1);
        }
    }
    return prod;
}
