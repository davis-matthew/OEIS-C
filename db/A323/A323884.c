/*
 * A323884  Sum of A322026 and its Dirichlet inverse.
 * https://oeis.org/A323884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A322026(n) + A323883(n), where A323883 is the Dirichlet
 *            inverse of A322026: b(1)=1, b(n) = -Sum_{d|n, d>1}
 *            A322026(d)*b(n/d). Computed by a divisor sieve
 * Range:     i8 n <= 1295, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 8715 (2026-11-08)
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

static seq_t *g_A = 0;
static seq_t *g_B = 0;
static size_t g_cap = 0;

static void build_tables(size_t N)
{
    if (g_A) { mem_free(g_A); g_A = 0; }
    if (g_B) { mem_free(g_B); g_B = 0; }
    g_A = mem_new(seq_t, N + 1);
    g_B = mem_new(seq_t, N + 1);
    g_cap = N;
    for (size_t d = 1; d <= N; ++d) g_A[d] = A322026_a((idx_t)d);
    /* g_B[m] accumulates S[m] = sum_{d>1,d|m} A(d)*B(m/d) until finalized */
    for (size_t m = 1; m <= N; ++m) {
        if (m == 1) g_B[1] = 1;
        else g_B[m] = -g_B[m];
        for (size_t d = 2; m * d <= N; ++d) {
            g_B[m * d] += g_A[d] * g_B[m];
        }
    }
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (N > g_cap) {
        size_t cap = N < 1024 ? 1024 : N;
        build_tables(cap);
    }
    return g_A[N] + g_B[N];
}
