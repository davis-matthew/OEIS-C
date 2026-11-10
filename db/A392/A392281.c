/*
 * A392281  a(n) = log_2(A392280(n)).
 * https://oeis.org/A392281
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A029837(A051903(n)) = ceil(log_2(max exponent in prime
 *            factorization of n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 105 terms through n = 10000 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    uint64_t maxe = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] > maxe) maxe = ex[i];
    }

    unsigned k = 0;
    uint64_t p = 1;
    while (p < maxe) {
        p <<= 1;
        ++k;
    }
    return (seq_t)k;
}
