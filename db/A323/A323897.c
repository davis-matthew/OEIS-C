/*
 * A323897  Lexicographically earliest sequence such that a(i) = a(j) =>
 *          A002487(i) = A002487(j) and A083254(i) = A083254(j), for all i, j
 *          >= 1.
 * https://oeis.org/A323897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pairs
 *            [A002487(n), A083254(n)], i.e. ids assigned in order of first
 *            appearance of each pair; A002487 by binary iteration, A083254(n)
 *            = 2*phi(n) - n
 * Range:     u8 n <= 309, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 109 terms through n = 9478 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const long long *g_s;
static const long long *g_t;

static unsigned long long stern_pair_rg(unsigned long long m) {
    unsigned long long x = 1, y = 0;
    while (m > 0) {
        if (m & 1) y += x; else x += y;
        m >>= 1;
    }
    return y;
}

static unsigned long long totient_rg(unsigned long long m) {
    unsigned long long r = m, p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            r -= r / p;
        }
    }
    if (m > 1) r -= r / m;
    return r;
}

static int cmp_rg(const void *pa, const void *pb) {
    size_t i = *(const size_t *)pa, j = *(const size_t *)pb;
    if (g_s[i] != g_s[j]) return g_s[i] < g_s[j] ? -1 : 1;
    if (g_t[i] != g_t[j]) return g_t[i] < g_t[j] ? -1 : 1;
    return i < j ? -1 : (i > j ? 1 : 0);
}

seq_t a(idx_t n) {
    size_t N = (size_t)n, k, g;
    long long *s = (long long *)malloc((N + 1) * sizeof(long long));
    long long *t = (long long *)malloc((N + 1) * sizeof(long long));
    size_t *ord = (size_t *)malloc(N * sizeof(size_t));
    size_t *rep = (size_t *)malloc((N + 1) * sizeof(size_t));
    unsigned char *isfirst = (unsigned char *)malloc(N + 1);
    size_t *id = (size_t *)malloc((N + 1) * sizeof(size_t));
    size_t result, cnt = 0, start;

    for (k = 1; k <= N; k++) {
        s[k] = (long long)stern_pair_rg(k);
        t[k] = 2 * (long long)totient_rg(k) - (long long)k;
        ord[k - 1] = k;
        isfirst[k] = 0;
    }
    g_s = s; g_t = t;
    qsort(ord, N, sizeof(size_t), cmp_rg);

    start = 0;
    for (g = 0; g < N; g++) {
        if (g == 0 || s[ord[g]] != s[ord[g - 1]] || t[ord[g]] != t[ord[g - 1]])
            start = g;
        rep[ord[g]] = ord[start];
        if (g == start) isfirst[ord[g]] = 1;
    }
    for (k = 1; k <= N; k++) {
        if (isfirst[k]) { cnt++; id[k] = cnt; }
    }
    result = id[rep[N]];

    free(s); free(t); free(ord); free(rep); free(isfirst); free(id);
    return (seq_t)result;
}
