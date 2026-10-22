/*
 * A321816  Sum of 12th powers of odd divisors of n.
 * https://oeis.org/A321816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma_12(odd part of n) (sum of 12th powers of odd
 *            divisors)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 6, u64 n <= 40, u128 n <= 1626,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Extract the odd part of n */
    uint64_t m = (uint64_t)n;
    while ((m & 1ULL) == 0)
        m >>= 1;

    if (m == 1)
        return (seq_t)1;

    /* Factorize the odd part */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t idx = 0; idx < cnt; ++idx) {
        seq_t p = (seq_t)primes[idx];
        uint64_t e = exps[idx];

        /* Compute p^12 */
        seq_t pow12 = (seq_t)1;
        for (int k = 0; k < 12; ++k)
            pow12 *= p;

        /* Compute term = 1 + pow12 + pow12^2 + ... + pow12^e */
        seq_t term  = (seq_t)1;
        seq_t power = (seq_t)1;
        for (uint64_t i = 1; i <= e; ++i) {
            power *= pow12;
            term += power;
        }

        result *= term;
    }

    return result;
}
