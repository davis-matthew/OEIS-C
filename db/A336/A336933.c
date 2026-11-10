/*
 * A336933  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A007733(i) = A007733(j), for all i, j >= 1.
 * https://oeis.org/A336933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that A007733(k) = A007733(n)'s first
 *            occurrence rank (restricted growth transform of A007733);
 *            computed by direct scan with order of 2 mod odd part of n
 * Range:     u8 n <= 1222, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8128 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *g_rgs = 0;
static uint64_t g_cap = 0;

static uint64_t A007733_ord(uint64_t n) {
    while (n % 2 == 0) n /= 2;
    if (n == 1) return 1;
    uint64_t p = 2 % n, k = 1;
    while (p != 1) {
        p = (p * 2) % n;
        k++;
    }
    return k;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    if (N > g_cap) {
        uint64_t cap = g_cap ? g_cap : 1024;
        while (cap < N) cap *= 2;
        uint64_t *map = (uint64_t *)calloc(cap + 2, sizeof(uint64_t));
        uint64_t *rg = (uint64_t *)malloc((cap + 1) * sizeof(uint64_t));
        uint64_t next = 0, i;
        for (i = 1; i <= cap; i++) {
            uint64_t o = A007733_ord(i);
            if (map[o] == 0) map[o] = ++next;
            rg[i] = map[o];
        }
        free(map);
        free(g_rgs);
        g_rgs = rg;
        g_cap = cap;
    }
    return (seq_t)g_rgs[N];
}
