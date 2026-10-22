/*
 * A367515  The number of unitary divisors of n that are exponentially odious
 *          numbers (A270428).
 * https://oeis.org/A367515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 2^(# of prime factors of n whose exponent has an odd *
 *            binary digit sum)
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
    if (n == 0) return 0;          /* undefined for index 0, return 0 */

    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    idx_t r = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (__builtin_parityll(exps[i]))
            ++r;

    return (seq_t)1 << r;          /* 2^r */
}
