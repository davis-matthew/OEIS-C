/*
 * A355009  Primes in A354975.
 * https://oeis.org/A355009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A354975(A354972(n)), i.e. the n-th prime among A354975(k)
 *            = Sum_{i=1..k} (prime(i+k) mod prime(i)), k = 1,2,...; computed
 *            by search with memoization
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 992 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint32_t *g_pr = NULL;
static size_t g_sz = 0, g_cap = 0;
static uint32_t g_cand = 3;

static void ensure_primes(size_t k)
{
    if (g_cap == 0) {
        g_cap = 64;
        g_pr = (uint32_t *)malloc(g_cap * sizeof(uint32_t));
        if (!g_pr) abort();
        g_pr[0] = 2;
        g_sz = 1;
    }
    while (g_sz < k) {
        uint32_t p = g_cand;
        bool ok = true;
        for (size_t i = 0; i < g_sz; ++i) {
            uint64_t q = g_pr[i];
            if (q * q > p) break;
            if (p % q == 0) { ok = false; break; }
        }
        g_cand += 2;
        if (!ok) continue;
        if (g_sz == g_cap) {
            g_cap <<= 1;
            g_pr = (uint32_t *)realloc(g_pr, g_cap * sizeof(uint32_t));
            if (!g_pr) abort();
        }
        g_pr[g_sz++] = p;
    }
}

static bool A355009_isprime(uint64_t v)
{
    if (v < 2) return false;
    if (v % 2 == 0) return v == 2;
    for (uint64_t d = 3; d * d <= v; d += 2)
        if (v % d == 0) return false;
    return true;
}

static uint64_t *g_found = NULL;
static size_t g_nfound = 0, g_fcap = 0;
static uint64_t g_k = 0;

seq_t a(idx_t n)
{
    while (g_nfound < (size_t)n) {
        g_k++;
        ensure_primes((size_t)(2 * g_k));
        uint64_t s = 0;
        for (uint64_t i = 1; i <= g_k; ++i)
            s += g_pr[i + g_k - 1] % g_pr[i - 1];
        if (A355009_isprime(s)) {
            if (g_nfound == g_fcap) {
                g_fcap = g_fcap ? g_fcap * 2 : 64;
                g_found = (uint64_t *)realloc(g_found, g_fcap * sizeof(uint64_t));
                if (!g_found) abort();
            }
            g_found[g_nfound++] = s;
        }
    }
    return (seq_t)g_found[(size_t)n - 1];
}
