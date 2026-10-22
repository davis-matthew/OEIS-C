/*
 * A371014  The number of divisors of n that are the sum of 2 squares.
 * https://oeis.org/A371014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative with a(p^e) = floor(e/2)+1 if p == 3 mod 4, else
 *            e+1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
    if (n == 1) return 1;

    /* small stack arrays; 10 000 has at most 6 distinct prime factors */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        seq_t term;
        if (p % 4 == 3)
            term = (seq_t)(exps[i] / 2 + 1);
        else
            term = (seq_t)(exps[i] + 1);
        result = result * term;
    }
    return result;
}
