/*
 * A389457  The sum of divisors of n that are terms in A048103 (that are not
 *          divisible by p^p for any prime p).
 * https://oeis.org/A389457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∑_{d|n, d∈A048103} d, multiplicative with a(p^e) =
 *            (p^(1+min(p-1,e)) - 1)/(p-1)
 * Range:     u8 n <= 48, u16 n <= 8505, u32 n <= 8505, u64 n <= 8505,
 *            u128 n <= 8505, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t t = (e < (p - 1)) ? e : (p - 1);
        uint64_t pow_exp = 1 + t;

        seq_t term;
        if (pow_exp == 2) {
            /* (p^2 - 1)/(p - 1) = p + 1 */
            term = (seq_t)p + 1;
        } else {
            seq_t p_pow = big_pow((seq_t)p, (seq_t)pow_exp);
            term = (p_pow - 1) / ((seq_t)p - 1);
        }

        result *= term;
    }
    return result;
}
