/*
 * A302776  a(1) = 1; for n>1, a(n) = n/(largest Fermi-Dirac factor of n).
 * https://oeis.org/A302776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / A223491(n), where A223491(n) is the largest divisor
 *            of n of the form p^(2^k)
 * Range:     u8 n <= 2519, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9635 (2026-11-08)
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
    if (n <= 1) return (seq_t)1;

    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t best = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t t = 1;
        while (t * 2 <= exps[i]) t <<= 1;
        uint64_t v = 1;
        for (uint64_t j = 0; j < t; ++j) v *= primes[i];
        if (v > best) best = v;
    }
    return (seq_t)((uint64_t)n / best);
}
