/*
 * A366074  The number of "Fermi-Dirac primes" (A050376) that are unitary
 *          divisors of n.
 * https://oeis.org/A366074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   additive with a(p^e) = A209229(e), i.e. a(n) = number of primes
 *            p whose exact exponent e in n is a power of 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 8375 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        if (e != 0 && (e & (e - 1)) == 0)
            ++result;
    }
    return result;
}
