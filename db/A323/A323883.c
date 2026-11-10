/*
 * A323883  Dirichlet inverse of A322026.
 * https://oeis.org/A323883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n) = -Sum_{d|n, d>1} A322026(d) * a(n/d) (Dirichlet
 *            inverse of A322026)
 * Range:     i8 n <= 3887, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 112 terms through n = 9448 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t A322026_a(idx_t n)
{
    unsigned v2 = 0;
    uint64_t x = n;
    while ((x & 1ULL) == 0) { v2++; x >>= 1; }

    unsigned v3 = 0;
    while (x % 3ULL == 0) { v3++; x /= 3ULL; }

    __uint128_t bound128 = ((__uint128_t)1ULL << v2);
    for (unsigned i = 0; i < v3; ++i) bound128 *= 3ULL;
    uint64_t bound = (uint64_t)bound128;

    uint64_t pow3[65];
    size_t pow3_len = 1;
    pow3[0] = 1;
    for (size_t i = 1; ; ++i) {
        __uint128_t next = (__uint128_t)pow3[i-1] * 3ULL;
        if (next > bound) break;
        pow3[i] = (uint64_t)next;
        pow3_len = i + 1;
    }

    seq_t count = 0;
    for (unsigned i = 0; ; ++i) {
        if (i >= 64) break;
        uint64_t term = 1ULL << i;
        if (term > bound) break;
        uint64_t remaining = bound / term;
        size_t jmax = 0;
        while (jmax + 1 < pow3_len && pow3[jmax + 1] <= remaining)
            ++jmax;
        count += (seq_t)(jmax + 1);
    }
    return count;
}

static seq_t *A323883_cache = 0;
static size_t A323883_cap = 0;

static void A323883_build(size_t N)
{
    if (A323883_cache) mem_free(A323883_cache);
    seq_t *fv = mem_new(seq_t, N + 1);
    seq_t *b = mem_new(seq_t, N + 1);
    for (size_t j = 1; j <= N; ++j) fv[j] = A322026_a((idx_t)j);
    for (size_t i = 1; i <= N; ++i) {
        if (i == 1) b[i] = 1;
        else b[i] = -b[i];
        for (size_t j = 2; i * j <= N; ++j)
            b[i * j] += fv[j] * b[i];
    }
    mem_free(fv);
    A323883_cache = b;
    A323883_cap = N;
}

seq_t a(idx_t n)
{
    size_t m = (size_t)n;
    if (m > A323883_cap) {
        size_t N = A323883_cap * 2;
        if (N < m) N = m;
        if (N < 64) N = 64;
        A323883_build(N);
    }
    return A323883_cache[m];
}
