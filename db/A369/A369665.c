/*
 * A369665  a(n) = 1 if n > 1 and A276085(A003415(n)) == n (mod 4), otherwise
 *          0, where A003415 is the arithmetic derivative, and A276085 is the
 *          primorial base log-function.
 * https://oeis.org/A369665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n > 1 and A276085(A003415(n)) == n (mod 4), else 0.
 *            A276085(m) mod 4 = v2(m) + 2*(number of odd prime factors of m
 *            with multiplicity) mod 4, since the product of primes below any
 *            odd prime is 2 mod 4
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 147 terms through n = 9663 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)0;

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* arithmetic derivative: sum (n/p)*e */
    uint64_t d = 0;
    for (size_t i = 0; i < cnt; ++i)
        d += ((uint64_t)n / pr[i]) * ex[i];

    uint64_t r = 0;
    if (d > 1) {
        uint64_t pr2[64], ex2[64];
        size_t cnt2 = 0;
        factor_u64(d, pr2, ex2, &cnt2);
        for (size_t i = 0; i < cnt2; ++i) {
            if (pr2[i] == 2)
                r += ex2[i];
            else
                r += 2 * ex2[i];
        }
    }
    return (seq_t)(((r & 3u) == ((uint64_t)n & 3u)) ? 1 : 0);
}
