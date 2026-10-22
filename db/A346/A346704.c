/*
 * A346704  Product of primes at even positions in the weakly increasing list
 *          (with multiplicity) of prime factors of n.
 * https://oeis.org/A346704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = product of primes at even positions in the weakly *
 *            increasing list of prime factors of n (with multiplicity)
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
    /* n == 0 is outside the OEIS offset; return 1 as a safe fallback */
    if (n <= 1)
        return 1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod = 1;
    idx_t pos = 1;                    /* 1‑based position in the factor list */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        for (uint64_t j = 0; j < e; ++j) {
            if ((pos & 1ULL) == 0)    /* even position? */
                prod *= (seq_t)p;
            ++pos;
        }
    }
    return prod;
}
