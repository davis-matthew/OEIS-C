/*
 * A382702  Indices k where b(k) > k, where b is the EKG sequence A064413.
 * https://oeis.org/A382702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th index k with A064413(k) > k; computed by simulating
 *            the EKG sequence (search with per-prime pointers)
 * Range:     u8 n <= 210, u16 n <= 9296, u32 n <= 9296, u64 n <= 9296,
 *            u128 n <= 9296, big any n
 * Validated: standard, pass, 98 terms through n = 9240 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *g_terms = NULL;
static size_t g_cnt = 0;
static size_t g_K = 0;

static void ekg_build(size_t K)
{
    size_t V = 4 * K + 100;
    uint32_t *spf = (uint32_t *)calloc(V + 1, sizeof(uint32_t));
    unsigned char *used = (unsigned char *)calloc(V + 1, 1);
    uint32_t *ptr = (uint32_t *)calloc(V + 1, sizeof(uint32_t));
    uint64_t *terms = (uint64_t *)malloc((K + 2) * sizeof(uint64_t));
    size_t cnt = 0;
    size_t i, j;
    uint32_t prev;
    if (!spf || !used || !ptr || !terms) exit(1);
    for (i = 2; i <= V; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= V; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    used[1] = 1; used[2] = 1;
    /* b(1)=1 not > 1; b(2)=2 not > 2 */
    prev = 2;
    for (i = 3; i <= K; i++) {
        uint32_t x = prev, best = 0xFFFFFFFFu;
        while (x > 1) {
            uint32_t p = spf[x];
            uint32_t m = ptr[p] ? ptr[p] : p;
            while (m <= V && used[m]) m += p;
            ptr[p] = m;
            if (m < best) best = m;
            while (x % p == 0) x /= p;
        }
        if (best > V) exit(1);
        used[best] = 1;
        prev = best;
        if (best > i) terms[cnt++] = i;
    }
    free(spf); free(used); free(ptr);
    free(g_terms);
    g_terms = terms;
    g_cnt = cnt;
    g_K = K;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (g_K == 0 || (size_t)n > g_cnt) {
        size_t K = g_K ? g_K * 2 : 4096;
        if (g_K == 0 && (size_t)n * 4 + 100 > K) K = (size_t)n * 4 + 100;
        ekg_build(K);
    }
    return (seq_t)g_terms[n - 1];
}
