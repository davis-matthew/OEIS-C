/*
 * A367990  Sum of the squarefree divisors of the largest unitary divisor of n
 *          that is a square.
 * https://oeis.org/A367990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative; a(p^e) = p+1 if e is even, and 1 otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* outside the offset; return 1 for safety */
        return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        if ((exps[i] & 1ULL) == 0ULL) {          /* exponent even? */
            result *= (seq_t)(primes[i] + 1);
        }
    }
    return result;
}
