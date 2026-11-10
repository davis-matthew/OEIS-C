/*
 * A299991  Numbers n for which A243822(n) > A000005(n).
 * https://oeis.org/A299991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m such that A010846(m) > 2*A000005(m), i.e.
 *            A243822(m) > A000005(m); found by an incremental search,
 *            counting numbers <= m whose prime factors all divide m by DFS
 *            over products of m's primes
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9127 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *a299991_terms = NULL;
static size_t a299991_len = 0, a299991_cap = 0;
static uint64_t a299991_next = 2;

static uint64_t a299991_dfs(const uint64_t *pr, int np, int i, uint64_t cur, uint64_t lim)
{
    uint64_t c = 1;
    for (int j = i; j < np; j++) {
        if (pr[j] > lim / cur) break;
        c += a299991_dfs(pr, np, j, cur * pr[j], lim);
    }
    return c;
}

static int a299991_test(uint64_t m)
{
    uint64_t pr[24];
    int np = 0;
    uint64_t d = 1, t = m;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            unsigned e = 0;
            while (t % p == 0) { t /= p; e++; }
            pr[np++] = p;
            d *= (e + 1);
        }
    }
    if (t > 1) { pr[np++] = t; d *= 2; }
    uint64_t c = a299991_dfs(pr, np, 0, 1, m);
    return c > 2 * d;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (a299991_len < (size_t)n) {
        if (a299991_test(a299991_next)) {
            if (a299991_len == a299991_cap) {
                a299991_cap = a299991_cap ? a299991_cap * 2 : 1024;
                a299991_terms = (uint64_t *)realloc(a299991_terms, a299991_cap * sizeof(uint64_t));
            }
            a299991_terms[a299991_len++] = a299991_next;
        }
        a299991_next++;
    }
    return (seq_t)a299991_terms[n - 1];
}
