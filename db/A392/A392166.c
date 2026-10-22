/*
 * A392166  The Dedekind psi function value of the smallest number whose
 *          square is divisible by n.
 * https://oeis.org/A392166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product_{p^e || n} (p+1) * p^{floor((e-1)/2)} where the
 *            product runs over the prime power factors p^e of n
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
    /* factorise n */
    size_t cnt;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];
        uint64_t exp = (e - 1) / 2;          /* floor((e-1)/2) */
        seq_t term   = seq_pow((seq_t)p, (idx_t)exp);
        term *= (seq_t)(p + 1);
        result *= term;
    }

    return result;
}
