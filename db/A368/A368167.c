/*
 * A368167  The largest unitary divisor of n that is a cubefull exponentially
 *          odd number (A335988).
 * https://oeis.org/A368167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product of p^e over all primes p^e in the factorization
 *            of n where e is odd and greater than 1; otherwise the factor
 *            contributes 1
 * Range:     u8 n <= 342, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        if ((e & 1U) && e > 1U) {
            seq_t powv = big_pow((seq_t)primes[i], (seq_t)e);
            result *= powv;
        }
    }
    return result;
}
