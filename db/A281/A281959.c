/*
 * A281959  a(n) = sigma_25(n), the sum of the 25th powers of the divisors of
 *          n.
 * https://oeis.org/A281959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sigma_25(n), the sum of the 25th powers of the divisors
 *            of n
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

static seq_t sigma25_of_u64(uint64_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Prime factorisation of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        /* Compute 1 + p^25 + p^(2*25) + ... + p^(e*25) */
        seq_t term   = (seq_t)1;
        seq_t pow25  = big_pow((seq_t)primes[i], (seq_t)25);
        seq_t cur    = (seq_t)1;          /* current p^(k*25) */

        for (uint64_t e = 1; e <= exps[i]; ++e) {
            cur *= pow25;                 /* p^(e*25) */
            term += cur;                  /* add to sigma */
        }

        result *= term;                   /* multiplicative property */
    }

    return result;
}

seq_t a(idx_t n)
{
    return sigma25_of_u64((uint64_t)n);
}
