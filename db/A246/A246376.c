/*
 * A246376  Permutation of natural numbers: a(1) = 1, a(2n) = 2 * a(n),
 *          a(2n-1) = 1 + (2 * a(A064989(2n-1)-1)).
 * https://oeis.org/A246376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2n) = 2*a(n), a(2n-1) = 1 + 2*a(A064989(2n-1)-1)
 * Range:     u8 n <= 108, u16 n <= 3850, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 97 terms through n = 9861 (2026-11-07)
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

static uint64_t A246376_prev_prime(uint64_t n)
{
    uint64_t p = n - 1;
    for (;; --p)
    {
        if (is_prime((idx_t)p)) return p;
    }
}

/* A064989 on a native integer (value never exceeds n) */
static uint64_t A246376_A064989(uint64_t n)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);
    uint64_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (pr[i] == 2) continue;
        uint64_t b = A246376_prev_prime(pr[i]);
        for (uint64_t e = 0; e < ex[i]; ++e) result *= b;
    }
    return result;
}

static seq_t A246376_rec(uint64_t n)
{
    unsigned k = 0;
    while ((n & 1u) == 0)
    {
        n >>= 1;
        k++;
    }
    seq_t r;
    if (n == 1)
    {
        r = 1;
    }
    else
    {
        uint64_t g = A246376_A064989(n) - 1;
        r = (seq_t)1 + (seq_t)2 * A246376_rec(g);
    }
    for (unsigned i = 0; i < k; ++i) r = r * 2;
    return r;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    return A246376_rec((uint64_t)n);
}
