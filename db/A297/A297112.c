/*
 * A297112  Möbius transform of A156552.
 * https://oeis.org/A297112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(2) = 1; a(2n+1) = 2*a(A064989(2n+1)), a(4n) =
 *            2*a(2n), a(4n+2) = a(2n+1)
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 136, u64 n <= 312,
 *            u128 n <= 726, big any n
 * Validated: standard, pass, 91 terms through n = 2040 (2026-11-08)
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

static idx_t A297112_prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p) {
        if (is_prime(p)) return p;
    }
}

/* A064989 computed in idx_t (value never exceeds n) */
static idx_t A297112_A064989(idx_t n)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);
    idx_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] == 2) continue;
        idx_t base = A297112_prev_prime((idx_t)pr[i]);
        for (uint64_t e = 0; e < ex[i]; ++e) result *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if ((n & 3) == 0) return (seq_t)2 * a(n / 2);
    if ((n & 3) == 2) return a(n / 2);
    return (seq_t)2 * a(A297112_A064989(n));
}
