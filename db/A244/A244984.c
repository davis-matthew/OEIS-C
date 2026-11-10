/*
 * A244984  Permutation of natural numbers: a(n) = A243283(A122111((2*n)-1)).
 * https://oeis.org/A244984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A243283(A122111(2n-1)); A122111 conjugates the partition
 *            of prime indices, and A243283(N) = 1 + #{m<=N : (largest prime
 *            factor of m)^2 divides m}, counted as 1 + sum over primes p of
 *            #(p-smooth k <= N/p^2)
 * Range:     u8 n <= 9, u16 n <= 26, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *g_pr = NULL;
static size_t g_sz = 0, g_cap = 0;

static uint64_t g_getp(size_t i)
{
    while (g_sz <= i) {
        if (g_sz == 0) {
            g_cap = 64;
            g_pr = (uint64_t *)malloc(g_cap * sizeof(uint64_t));
            if (!g_pr) abort();
            g_pr[g_sz++] = 2;
            continue;
        }
        uint64_t c = g_pr[g_sz - 1] + (g_sz == 1 ? 1 : 2);
        for (;; c += 2) {
            bool ok = true;
            for (size_t k = 0; k < g_sz; ++k) {
                uint64_t q = g_pr[k];
                if (q * q > c) break;
                if (c % q == 0) { ok = false; break; }
            }
            if (ok) break;
        }
        if (g_sz == g_cap) {
            g_cap *= 2;
            g_pr = (uint64_t *)realloc(g_pr, g_cap * sizeof(uint64_t));
            if (!g_pr) abort();
        }
        g_pr[g_sz++] = c;
    }
    return g_pr[i];
}

/* count integers in [1,x] whose prime factors are among the first i+1 primes */
static seq_t g_psi(seq_t x, size_t i)
{
    if (x == 0) return 0;
    uint64_t p = g_getp(i);
    if ((seq_t)p >= x) return x;
    if (i == 0) {
        seq_t s = 0;
        while (x > 0) { s += 1; x /= 2; }
        return s;
    }
    seq_t s = 0;
    seq_t t = x;
    while (t > 0) {
        s += g_psi(t, i - 1);
        t /= (seq_t)p;
    }
    return s;
}

static seq_t g_A243283(seq_t N)
{
    if (N == 0) return 0;
    seq_t total = 1;
    for (size_t i = 0;; ++i) {
        seq_t p = (seq_t)g_getp(i);
        seq_t p2 = p * p;
        if (p2 > N) break;
        total += g_psi(N / p2, i);
    }
    return total;
}

seq_t a(idx_t n)
{
    uint64_t m = 2 * (uint64_t)n - 1;
    uint64_t parts[70];
    size_t cnt = 0;
    for (size_t i = 0; m > 1; ++i) {
        uint64_t p = g_getp(i);
        if (p * p > m) {
            /* m is prime; find its index */
            size_t k = i;
            while (g_getp(k) < m) ++k;
            parts[cnt++] = (uint64_t)k + 1;
            m = 1;
            break;
        }
        while (m % p == 0) {
            m /= p;
            parts[cnt++] = (uint64_t)i + 1;
        }
    }
    if (cnt == 0) return 1;
    /* insertion sort ascending */
    for (size_t i = 1; i < cnt; ++i) {
        uint64_t v = parts[i];
        size_t j = i;
        while (j > 0 && parts[j - 1] > v) { parts[j] = parts[j - 1]; --j; }
        parts[j] = v;
    }
    uint64_t maxpart = parts[cnt - 1];
    seq_t N = 1;
    size_t s = 0;
    for (uint64_t j = 1; j <= maxpart; ++j) {
        while (s < cnt && parts[s] < j) ++s;
        size_t c = cnt - s;
        N *= (seq_t)g_getp(c - 1);
    }
    return g_A243283(N);
}
