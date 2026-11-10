/*
 * A336390  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A336467(i) = A336467(j) and A336158(i) = A336158(j), for all i,
 *          j >= 1.
 * https://oeis.org/A336390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pairs
 *            [A336467(n), A336158(n)]; computed by scanning k=1..n and
 *            ranking pairs by first occurrence
 * Range:     u8 n <= 950, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9829 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static uint64_t *A336390_res = NULL;
static uint64_t *A336390_hf = NULL;
static uint64_t *A336390_hg = NULL;
static uint64_t *A336390_hv = NULL;
static uint64_t A336390_done = 0;

static void A336390_pair(uint64_t k, uint64_t *pf, uint64_t *pg)
{
    static const uint64_t sp[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    uint64_t m = k;
    while ((m & 1) == 0) m >>= 1;
    uint64_t f = 1;
    unsigned ex[20];
    int cnt = 0;
    uint64_t d = 3;
    while (m > 1) {
        uint64_t p;
        if (d * d > m) { p = m; } else {
            if (m % d != 0) { d += 2; continue; }
            p = d;
        }
        unsigned e = 0;
        while (m % p == 0) { m /= p; e++; }
        uint64_t o = p + 1;
        while ((o & 1) == 0) o >>= 1;
        for (unsigned i = 0; i < e; i++) f *= o;
        if (cnt < 16) ex[cnt++] = e;
        d += 2;
    }
    for (int i = 0; i < cnt; i++)
        for (int j = i + 1; j < cnt; j++)
            if (ex[i] < ex[j]) { unsigned t = ex[i]; ex[i] = ex[j]; ex[j] = t; }
    uint64_t g = 1;
    for (int i = 0; i < cnt; i++)
        for (unsigned e = 0; e < ex[i]; e++) g *= sp[i];
    *pf = f;
    *pg = g;
}

static void A336390_ensure(uint64_t N)
{
    if (N <= A336390_done) return;
    uint64_t cap = N;
    if (cap < 2 * A336390_done) cap = 2 * A336390_done;
    if (cap < 1024) cap = 1024;
    free(A336390_res); free(A336390_hf); free(A336390_hg); free(A336390_hv);
    uint64_t H = 1;
    while (H < 2 * cap) H <<= 1;
    A336390_res = (uint64_t *)malloc((cap + 1) * sizeof(uint64_t));
    A336390_hf = (uint64_t *)calloc(H, sizeof(uint64_t));
    A336390_hg = (uint64_t *)calloc(H, sizeof(uint64_t));
    A336390_hv = (uint64_t *)calloc(H, sizeof(uint64_t));
    uint64_t next = 1;
    for (uint64_t k = 1; k <= cap; k++) {
        uint64_t f, g;
        A336390_pair(k, &f, &g);
        uint64_t h = (f * 0x9E3779B97F4A7C15ULL) ^ (g * 0xC2B2AE3D27D4EB4FULL);
        h ^= h >> 29;
        uint64_t pos = h & (H - 1);
        while (A336390_hv[pos] != 0 && !(A336390_hf[pos] == f && A336390_hg[pos] == g))
            pos = (pos + 1) & (H - 1);
        if (A336390_hv[pos] == 0) {
            A336390_hf[pos] = f;
            A336390_hg[pos] = g;
            A336390_hv[pos] = next++;
        }
        A336390_res[k] = A336390_hv[pos];
    }
    A336390_done = cap;
}

seq_t a(idx_t n)
{
    A336390_ensure((uint64_t)n);
    return (seq_t)A336390_res[(uint64_t)n];
}
