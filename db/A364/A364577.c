/*
 * A364577  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A294898(i) = A294898(j) and A329697(i) = A329697(j), for all i,
 *          j >= 1.
 * https://oeis.org/A364577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of the pairs [A294898(n),
 *            A329697(n)]; computed by scanning first occurrences
 * Range:     u8 n <= 215, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 98 terms through n = 8178 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A294898_sigma_of(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t sum_p = 1;
            seq_t p_pow = 1;
            while (m % p == 0) {
                m /= p;
                p_pow *= (seq_t)p;
                sum_p += p_pow;
            }
            result *= sum_p;
        }
    }
    if (m > 1) result *= (seq_t)(1 + m);
    return result;
}

static seq_t A294898_a(idx_t n)
{
    seq_t pop = (seq_t)__builtin_popcountll((unsigned long long)n);
    return (seq_t)2 * (seq_t)n - A294898_sigma_of(n) - pop;
}

static uint64_t A329697_lpf(uint64_t m)
{
    uint64_t largest = 0;
    while ((m & 1ULL) == 0) {
        largest = 2;
        m >>= 1;
    }
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0) m /= f;
        }
    }
    if (m > 1) largest = m;
    return largest;
}

static uint64_t A329697_a(uint64_t n)
{
    uint64_t c = 0;
    while (n > 2) {
        if ((n & (n - 1)) == 0) break;
        while ((n & 1) == 0) n >>= 1;
        if (n == 1) break;
        uint64_t l = A329697_lpf(n);
        n = n - n / l;
        c++;
    }
    return c;
}

static seq_t *g_A = 0;
static uint64_t *g_B = 0;
static uint64_t *g_rank = 0;
static uint64_t g_cap = 0;
static uint64_t g_filled = 0;
static uint64_t g_count = 0;

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N == 0) return 0;
    if (N > g_cap) {
        uint64_t nc = g_cap * 2;
        if (nc < N) nc = N;
        if (nc < 1024) nc = 1024;
        if (g_A) mem_free(g_A);
        if (g_B) mem_free(g_B);
        if (g_rank) mem_free(g_rank);
        g_A = mem_new(seq_t, nc + 1);
        g_B = mem_new(uint64_t, nc + 1);
        g_rank = mem_new(uint64_t, nc + 1);
        g_cap = nc;
        g_filled = 0;
        g_count = 0;
    }
    while (g_filled < N) {
        uint64_t i = g_filled + 1;
        seq_t v = A294898_a((idx_t)i);
        uint64_t w = A329697_a(i);
        uint64_t found = 0;
        for (uint64_t j = 0; j < g_count; j++) {
            if (g_B[j] == w && g_A[j] == v) {
                found = j + 1;
                break;
            }
        }
        if (!found) {
            g_A[g_count] = v;
            g_B[g_count] = w;
            g_count++;
            found = g_count;
        }
        g_rank[i] = found;
        g_filled = i;
    }
    return (seq_t)g_rank[N];
}
