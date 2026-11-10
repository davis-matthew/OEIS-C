/*
 * A252735  a(1) = 0; for n > 1: a(2n) = a(n), a(2n+1) = 1 + a(A064989(n)).
 * https://oeis.org/A252735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; a(2n) = a(n), a(2n+1) = 1 + a(A064989(2n+1)); computed
 *            iteratively
 * Range:     u8 n <= 1620, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 124 terms through n = 9989 (2026-11-07)
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

static uint64_t A064989_prev_prime(uint64_t n)
{
    uint64_t p = n - 1;
    for (;; --p) {
        if (is_prime((idx_t)p)) return p;
    }
}

static uint64_t A064989_u64(uint64_t n)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    uint64_t result = 1;
    factor_u64(n, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] == 2) continue;
        uint64_t b = A064989_prev_prime(pr[i]);
        for (uint64_t e = 0; e < ex[i]; ++e) result *= b;
    }
    return result;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    uint64_t count = 0;
    if (m < 2) return 0;
    while (m > 1) {
        if ((m & 1) == 0) {
            m >>= 1;
        } else {
            ++count;
            m = A064989_u64(m);
        }
    }
    return (seq_t)count;
}
