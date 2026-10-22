/*
 * A367870  a(n) = Sum_{d|n, d odd} (n-d).
 * https://oeis.org/A367870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d | n, d odd} (n - d)
 * Range:     u8 n <= 62, u16 n <= 3464, u32 n <= 10000, u64 n <= 10000,
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
    if (n == 0) return 0;                           /* safety for n = 0 */

    /* Compute the odd part of n */
    idx_t odd = n;
    while ((odd & 1ULL) == 0ULL)
        odd >>= 1;

    /* Factor the odd part */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(odd, primes, exps, &cnt);

    /* Compute number of odd divisors and sum of odd divisors */
    seq_t odd_cnt = 1;
    seq_t sum_od   = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        uint64_t exp = exps[i];

        odd_cnt *= ((seq_t)exp + 1);

        seq_t term = 1;      /* sum of powers of p: 1 + p + ... + p^exp */
        seq_t powp = 1;
        for (idx_t k = 1; k <= exp; ++k) {
            powp *= p;
            term += powp;
        }
        sum_od *= term;
    }

    /* a(n) = n * (number of odd divisors) - (sum of odd divisors) */
    return (seq_t)n * odd_cnt - sum_od;
}
