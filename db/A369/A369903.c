/*
 * A369903  a(n) = Sum_{p|n, p prime} n^sigma(n/p).
 * https://oeis.org/A369903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{p | n, p prime} n^σ(n/p) (sum over distinct prime
 *            divisors)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* offset: a(1) = 0 */
    if (n <= 1)
        return (seq_t)0;

    /* factor n and collect distinct prime divisors */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        idx_t m = n / (idx_t)p;              /* n/p  */
        seq_t sigma = sigma_of(m);           /* σ(n/p)  */
        seq_t pow = ipow((seq_t)n, sigma);
        result += pow;
    }
    return result;
}
