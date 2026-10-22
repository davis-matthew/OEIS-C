/*
 * A381214  a(n) is the difference between the largest and smallest element of
 *          the set of bases and exponents (including exponents = 1) in the
 *          prime factorization of n.
 * https://oeis.org/A381214
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max{p,e | n = ∏ p^e } – min{p,e} * (include exponents e,
 *            e may be 1)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n < 2)
        return 0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t minv = (uint64_t)-1;
    uint64_t maxv = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t v = primes[i];
        if (v < minv) minv = v;
        if (v > maxv) maxv = v;

        v = exps[i];
        if (v < minv) minv = v;
        if (v > maxv) maxv = v;
    }

    return (seq_t)(maxv - minv);
}
