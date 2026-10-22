/*
 * A399831  The number of divisors d of n such that gcud(d, n/d) is an
 *          exponentially odd number (A268335), where gcud is the greatest
 *          common unitary divisor.
 * https://oeis.org/A399831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) is multiplicative with a(p^e) = e if e % 4 == 0 else e + 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n >= 1 for the OEIS offset; below that behaviour is irrelevant */
    if (n <= 1)
        return (seq_t)1;

    /* 64‑bit integer factorisation – 128 slots are ample for all n ≤ 2^64 */
    uint64_t primes[128];
    uint64_t exps[128];
    size_t cnt;

    /* factor_u64 breaks n into prime powers */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        seq_t factor = (e % 4 == 0) ? (seq_t)e : (seq_t)(e + 1);
        result *= factor;
    }
    return result;
}
