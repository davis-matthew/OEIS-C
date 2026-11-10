/*
 * A322808  Lexicographically earliest such sequence a that a(i) = a(j) =>
 *          f(i) = f(j) for all i, j, where f(n) = 0 if n is a squarefree
 *          number > 2, and f(n) = A097246(n) for all other numbers.
 * https://oeis.org/A322808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank of first occurrence of f(n) among distinct values of
 *            f(1..n), where f(m)=0 if m is squarefree and >2, else A097246(m)
 *            (restricted-growth labelling, computed by sieve-like scan)
 * Range:     u8 n <= 960, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9662 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A322808_isp(uint64_t n)
{
    if (n < 2) return false;
    if (n < 4) return true;
    if ((n & 1) == 0 || n % 3 == 0) return false;
    for (uint64_t d = 5; d * d <= n; d += 6)
        if (n % d == 0 || n % (d + 2) == 0) return false;
    return true;
}

static uint64_t A322808_nextp(uint64_t p)
{
    uint64_t c = p + 1;
    while (!A322808_isp(c)) ++c;
    return c;
}

static uint64_t A322808_f(uint64_t m)
{
    uint64_t res = 1, orig = m;
    bool sqf = true;
    for (uint64_t d = 2; d * d <= m; d += (d == 2 ? 1 : 2)) {
        if (m % d) continue;
        int e = 0;
        while (m % d == 0) { m /= d; ++e; }
        if (e > 1) sqf = false;
        if (e & 1) res *= d;
        for (int i = 0; i < e / 2; ++i) res *= A322808_nextp(d);
    }
    if (m > 1) res *= m;
    if (sqf && orig > 2) return 0;
    return res;
}

static uint64_t *A322808_lab = 0;
static uint64_t A322808_N = 0;

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    if (nn > A322808_N) {
        uint64_t N = nn;
        if (N < 2 * A322808_N) N = 2 * A322808_N;
        if (N < 1024) N = 1024;
        if (A322808_lab) free(A322808_lab);
        A322808_lab = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
        uint64_t *seen = (uint64_t *)calloc(N + 2, sizeof(uint64_t));
        uint64_t cnt = 0;
        A322808_lab[0] = 0;
        for (uint64_t m = 1; m <= N; ++m) {
            uint64_t v = A322808_f(m);
            if (seen[v] == 0) seen[v] = ++cnt;
            A322808_lab[m] = seen[v];
        }
        free(seen);
        A322808_N = N;
    }
    return (seq_t)A322808_lab[nn];
}
