/*
 * A390663  The sum of the divisors of the smallest cube divisible by n.
 * https://oeis.org/A390663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sum of divisors of the smallest cube divisible by n. It
 *            is multiplicative: a(p^e) = (p^(e' + 1) - 1)/(p - 1), where e' =
 *            e + ((3 - e % 3) % 3) is the smallest exponent ≥ e that is a
 *            multiple of 3
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 1108, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    /* n is indexed from 1; for safety treat n==0 as 0 */
    if (n == 0) return 0;

    /* Temporary arrays for prime factors of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factor n */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* If n has no prime factors (i.e., n == 1) the answer is 1 */
    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Compute e' : next multiple of 3 not less than e */
        uint64_t rem = e % 3;
        uint64_t add = (3 - rem) % 3;
        uint64_t ep = e + add;        /* exponent of p in the smallest cube */

        /* sigma(p^ep) = (p^(ep+1) - 1) / (p - 1) */
        seq_t num   = pow_uint(p, ep + 1) - (seq_t)1;
        seq_t denom = (seq_t)(p - 1);
        seq_t term  = num / denom;

        result *= term;
    }

    return result;
}
