/*
 * A371730  Indices of records in A371729: numbers k with A371729(k) >
 *          A371729(m) for all m < k.
 * https://oeis.org/A371730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A371729(k) > A371729(m) for all m < k;
 *            computed by CRT-enumerating base residues per composite m,
 *            counting, and scanning for records (limit doubled until enough)
 * Range:     u8 n <= 14, u16 n <= 53, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: standard, pass, 65 terms through n = 68 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint32_t *A371730_rec = NULL;
static size_t A371730_nrec = 0;

static uint64_t A371730_pw(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = r * b % m;
        b = b * b % m;
        e >>= 1;
    }
    return r;
}

static uint64_t A371730_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint32_t A371730_proot(uint32_t p, const uint32_t *spf)
{
    uint32_t fac[16];
    int nf = 0;
    uint32_t t = p - 1;
    while (t > 1) {
        uint32_t q = spf[t];
        fac[nf++] = q;
        while (t % q == 0) t /= q;
    }
    for (uint32_t g = 2; g < p; ++g) {
        bool ok = true;
        for (int i = 0; i < nf; ++i)
            if (A371730_pw(g, (p - 1) / fac[i], p) == 1) { ok = false; break; }
        if (ok) return g;
    }
    return 1;
}

static void A371730_build(uint32_t N)
{
    uint32_t *spf = (uint32_t*)calloc((size_t)N + 1, sizeof(uint32_t));
    uint32_t *proot = (uint32_t*)calloc((size_t)N + 1, sizeof(uint32_t));
    uint32_t *cnt = (uint32_t*)calloc((size_t)N + 1, sizeof(uint32_t));
    for (uint32_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            for (uint32_t j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    size_t cap = 16;
    uint64_t *bufA = (uint64_t*)malloc(cap * sizeof(uint64_t));
    uint64_t *bufB = (uint64_t*)malloc(cap * sizeof(uint64_t));
    uint64_t *bufC = (uint64_t*)malloc(cap * sizeof(uint64_t));

    for (uint32_t m = 4; m < N; ++m) {
        if (spf[m] == m) continue;
        /* factor and total size */
        uint64_t qs[16], ps[16], ds[16];
        int nq = 0;
        uint32_t t = m;
        uint64_t total = 1;
        while (t > 1) {
            uint32_t p = spf[t];
            uint64_t q = 1;
            while (t % p == 0) { t /= p; q *= p; }
            uint64_t phi = q / p * (p - 1);
            uint64_t d = (p == 2) ? 1 : A371730_gcd(m - 1, phi);
            qs[nq] = q; ps[nq] = p; ds[nq] = d; ++nq;
            total *= d;
        }
        if (total > cap) {
            while (cap < total) cap *= 2;
            bufA = (uint64_t*)realloc(bufA, cap * sizeof(uint64_t));
            bufB = (uint64_t*)realloc(bufB, cap * sizeof(uint64_t));
            bufC = (uint64_t*)realloc(bufC, cap * sizeof(uint64_t));
        }
        uint64_t *cur = bufA, *nxt = bufB;
        size_t ncur = 1;
        cur[0] = 0;
        uint64_t M = 1;
        for (int i = 0; i < nq; ++i) {
            uint64_t q = qs[i], p = ps[i], d = ds[i];
            uint64_t phi = q / p * (p - 1);
            /* subgroup of order d */
            bufC[0] = 1 % q;
            if (d > 1) {
                if (proot[p] == 0) {
                    uint32_t g = A371730_proot((uint32_t)p, spf);
                    if (q != p && A371730_pw(g, p - 1, p * p) == 1) g += (uint32_t)p;
                    proot[p] = g;
                }
                uint64_t h = A371730_pw(proot[p], phi / d, q);
                uint64_t x = 1;
                for (uint64_t j = 0; j < d; ++j) {
                    bufC[j] = x;
                    x = x * h % q;
                }
            }
            uint64_t inv = A371730_pw(M % q, phi - 1, q);
            if (q == 1) inv = 0;
            size_t nn = 0;
            for (size_t a = 0; a < ncur; ++a) {
                uint64_t av = cur[a];
                uint64_t ar = av % q;
                for (uint64_t j = 0; j < d; ++j) {
                    uint64_t b = bufC[j];
                    uint64_t k = ((b + q - ar) % q) * inv % q;
                    nxt[nn++] = av + M * k;
                }
            }
            uint64_t *tmp = cur; cur = nxt; nxt = tmp;
            ncur = nn;
            M *= q;
        }
        for (size_t a = 0; a < ncur; ++a) {
            uint64_t r = cur[a];
            for (uint64_t k = r + m; k <= N; k += m)
                cnt[k]++;
        }
    }

    free(A371730_rec);
    A371730_rec = (uint32_t*)malloc(((size_t)N + 1) * sizeof(uint32_t));
    A371730_nrec = 0;
    int64_t best = -1;
    for (uint32_t k = 2; k <= N; ++k) {
        if ((int64_t)cnt[k] > best) {
            best = cnt[k];
            A371730_rec[A371730_nrec++] = k;
        }
    }
    free(bufA); free(bufB); free(bufC);
    free(spf); free(proot); free(cnt);
}

seq_t a(idx_t n)
{
    static uint32_t N = 0;
    if (n < 1) return 0;
    while (A371730_nrec < (size_t)n && (N == 0 || N < (1u << 30))) {
        N = (N == 0) ? 4096u : N * 2u;
        A371730_build(N);
    }
    if (A371730_nrec < (size_t)n) return 0;
    return (seq_t)A371730_rec[n - 1];
}
