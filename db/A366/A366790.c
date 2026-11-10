/*
 * A366790  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A366789(i) = A366789(j) and A336158(i) = A336158(j), for all i,
 *          j >= 1.
 * https://oeis.org/A366790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of the pairs [A366789(n),
 *            A336158(n)]; computed by sieve and first-occurrence labelling
 * Range:     u8 n <= 1270, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 114 terms through n = 9525 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A366790_lab = NULL;
static uint64_t A366790_cap = 0;

static uint64_t A366790_oddpart(uint64_t x)
{
    while (x && (x & 1) == 0) x >>= 1;
    return x;
}

static void A366790_build(uint64_t N)
{
    static const uint64_t sp[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    uint32_t *spf = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    uint32_t *pi = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    uint64_t *lab = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    uint64_t hs = 16;
    while (hs < 2 * N + 2) hs <<= 1;
    uint64_t *kx = (uint64_t *)calloc(hs, sizeof(uint64_t));
    uint64_t *ky = (uint64_t *)calloc(hs, sizeof(uint64_t));
    uint64_t *kl = (uint64_t *)calloc(hs, sizeof(uint64_t));
    if (!spf || !pi || !lab || !kx || !ky || !kl) {
        free(spf); free(pi); free(lab); free(kx); free(ky); free(kl);
        return;
    }
    uint32_t cnt = 0;
    for (uint64_t i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            cnt++;
            pi[i] = cnt;
            for (uint64_t j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    uint64_t next = 0;
    for (uint64_t i = 1; i <= N; i++) {
        uint64_t x = 1, m = i;
        uint64_t ex[64];
        size_t ne = 0;
        uint64_t odd_p_exps[64];
        size_t no = 0;
        (void)ex; (void)ne;
        while (m > 1) {
            uint64_t p = spf[m];
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            uint64_t b = A366790_oddpart(pi[p]);
            for (int k = 0; k < e; k++) x *= b;
            if (p != 2) odd_p_exps[no++] = (uint64_t)e;
        }
        for (size_t a = 0; a < no; a++)
            for (size_t b = a + 1; b < no; b++)
                if (odd_p_exps[a] < odd_p_exps[b]) {
                    uint64_t t = odd_p_exps[a];
                    odd_p_exps[a] = odd_p_exps[b];
                    odd_p_exps[b] = t;
                }
        uint64_t y = 1;
        for (size_t a = 0; a < no && a < 16; a++)
            for (uint64_t k = 0; k < odd_p_exps[a]; k++) y *= sp[a];
        uint64_t h = (x * 0x9E3779B97F4A7C15ULL) ^ (y * 0xC2B2AE3D27D4EB4FULL);
        h ^= h >> 29;
        h &= hs - 1;
        while (kl[h] && !(kx[h] == x && ky[h] == y)) h = (h + 1) & (hs - 1);
        if (!kl[h]) {
            kl[h] = ++next;
            kx[h] = x;
            ky[h] = y;
        }
        lab[i] = kl[h];
    }
    free(spf); free(pi); free(kx); free(ky); free(kl);
    free(A366790_lab);
    A366790_lab = lab;
    A366790_cap = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if ((uint64_t)n > A366790_cap) {
        uint64_t N = (uint64_t)n;
        if (N < 2 * A366790_cap) N = 2 * A366790_cap;
        if (N < 1024) N = 1024;
        A366790_build(N);
        if ((uint64_t)n > A366790_cap) A366790_build((uint64_t)n);
    }
    return (seq_t)A366790_lab[n];
}
