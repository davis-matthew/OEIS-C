/*
 * A355958  Positions where primes occur in A104589.
 * https://oeis.org/A355958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. Generate A104589 by its definition, a(1) = 1 and *
 *            A104589(m) = A104589(m-1) + (sum of earlier terms that are prime
 *            or 1), * using our own multi-precision integers. a(n) is the
 *            index m of the n-th * prime term. Primality: exact 64-bit test
 *            while the value fits, otherwise * trial division...
 * Range:     u8 n <= 19, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

static bool A355958_ge(const uint32_t *x, const uint32_t *N, int k)
{
    for (int i = k - 1; i >= 0; --i) {
        if (x[i] != N[i]) return x[i] > N[i];
    }
    return true;
}

static void A355958_sub(uint32_t *x, const uint32_t *N, int k)
{
    int64_t br = 0;
    for (int i = 0; i < k; ++i) {
        int64_t d = (int64_t)x[i] - (int64_t)N[i] - br;
        if (d < 0) { d += ((int64_t)1 << 32); br = 1; } else br = 0;
        x[i] = (uint32_t)d;
    }
}

/* out = x + y mod N, where x and y < N */
static void A355958_addmod(uint32_t *out, const uint32_t *x, const uint32_t *y,
                           const uint32_t *N, int k)
{
    uint64_t c = 0;
    for (int i = 0; i < k; ++i) {
        c += (uint64_t)x[i] + y[i];
        out[i] = (uint32_t)c;
        c >>= 32;
    }
    if (c || A355958_ge(out, N, k)) A355958_sub(out, N, k);
}

/* Montgomery product: out = a*b*R^-1 mod N, with t holding k+2 limbs of scratch */
static void A355958_mm(uint32_t *out, const uint32_t *A, const uint32_t *B,
                       const uint32_t *N, int k, uint32_t n0inv, uint32_t *t)
{
    memset(t, 0, sizeof(uint32_t) * (size_t)(k + 2));
    for (int i = 0; i < k; ++i) {
        uint64_t cs, C = 0;
        for (int j = 0; j < k; ++j) {
            cs = (uint64_t)t[j] + (uint64_t)A[j] * B[i] + C;
            t[j] = (uint32_t)cs; C = cs >> 32;
        }
        cs = (uint64_t)t[k] + C;
        t[k] = (uint32_t)cs; t[k + 1] = (uint32_t)(cs >> 32);
        uint32_t m = t[0] * n0inv;
        cs = (uint64_t)t[0] + (uint64_t)m * N[0];
        C = cs >> 32;
        for (int j = 1; j < k; ++j) {
            cs = (uint64_t)t[j] + (uint64_t)m * N[j] + C;
            t[j - 1] = (uint32_t)cs; C = cs >> 32;
        }
        cs = (uint64_t)t[k] + C;
        t[k - 1] = (uint32_t)cs; C = cs >> 32;
        t[k] = t[k + 1] + (uint32_t)C;
    }
    if (t[k] || A355958_ge(t, N, k)) A355958_sub(t, N, k);
    memcpy(out, t, sizeof(uint32_t) * (size_t)k);
}

/* Miller-Rabin on an odd N of k limbs (N > 2^64), first 20 prime bases */
static bool A355958_mr(const uint32_t *N, int k)
{
    static const unsigned P[20] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
                                   41, 43, 47, 53, 59, 61, 67, 71};
    uint32_t *buf = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(5 * k + 2));
    uint32_t *one = buf, *mone = buf + k, *bR = buf + 2 * k, *x = buf + 3 * k,
             *t = buf + 4 * k;
    uint32_t inv = N[0];
    for (int i = 0; i < 5; ++i) inv *= 2u - N[0] * inv;
    uint32_t n0inv = (uint32_t)0 - inv;

    /* one = R mod N, found by doubling 1 a total of 32k times */
    memset(one, 0, sizeof(uint32_t) * (size_t)k);
    one[0] = 1;
    for (long r = 0; r < 32L * k; ++r) A355958_addmod(one, one, one, N, k);
    memcpy(mone, N, sizeof(uint32_t) * (size_t)k);
    A355958_sub(mone, one, k);             /* mone = N - R mod N = -1 in Montgomery form */

    int top = 32 * k - 1;
    while (!((N[top >> 5] >> (top & 31)) & 1u)) --top;
    int s = 1;
    while (!((N[s >> 5] >> (s & 31)) & 1u)) ++s;   /* N-1 = d * 2^s */

    bool result = true;
    for (int b = 0; b < 20 && result; ++b) {
        memset(bR, 0, sizeof(uint32_t) * (size_t)k);
        for (unsigned c = 0; c < P[b]; ++c) A355958_addmod(bR, bR, one, N, k);
        memcpy(x, bR, sizeof(uint32_t) * (size_t)k);
        for (int i = top - 1; i >= s; --i) {
            A355958_mm(x, x, x, N, k, n0inv, t);
            if ((N[i >> 5] >> (i & 31)) & 1u) A355958_mm(x, x, bR, N, k, n0inv, t);
        }
        if (memcmp(x, one, sizeof(uint32_t) * (size_t)k) == 0 ||
            memcmp(x, mone, sizeof(uint32_t) * (size_t)k) == 0) continue;
        bool ok = false;
        for (int r = 1; r < s; ++r) {
            A355958_mm(x, x, x, N, k, n0inv, t);
            if (memcmp(x, mone, sizeof(uint32_t) * (size_t)k) == 0) { ok = true; break; }
        }
        if (!ok) result = false;
    }
    free(buf);
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* small primes below 1000 for trial division by residues */
    int sp[200], np = 0;
    {
        bool comp[1000];
        memset(comp, 0, sizeof(comp));
        for (int i = 2; i < 1000; ++i) {
            if (comp[i]) continue;
            sp[np++] = i;
            for (int j = i * i; j < 1000; j += i) comp[j] = true;
        }
    }
    uint32_t rc[200], rs[200];
    for (int i = 0; i < np; ++i) { rc[i] = 1; rs[i] = 1; }

    int L = 8;
    uint32_t *cur = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    uint32_t *S = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    cur[0] = 1; S[0] = 1;
    idx_t count = 0;
    idx_t i = 1;

    for (;;) {
        ++i;
        if (cur[L - 1] | S[L - 1]) {
            int NL = 2 * L;
            cur = (uint32_t *)realloc(cur, sizeof(uint32_t) * (size_t)NL);
            S = (uint32_t *)realloc(S, sizeof(uint32_t) * (size_t)NL);
            memset(cur + L, 0, sizeof(uint32_t) * (size_t)L);
            memset(S + L, 0, sizeof(uint32_t) * (size_t)L);
            L = NL;
        }
        /* cur += S */
        uint64_t c = 0;
        for (int j = 0; j < L; ++j) {
            c += (uint64_t)cur[j] + S[j];
            cur[j] = (uint32_t)c;
            c >>= 32;
        }
        for (int j = 0; j < np; ++j) rc[j] = (rc[j] + rs[j]) % (uint32_t)sp[j];

        int k = L;
        while (k > 1 && cur[k - 1] == 0) --k;
        bool pr;
        if (k <= 2) {
            uint64_t v = ((uint64_t)(k == 2 ? cur[1] : 0) << 32) | cur[0];
            pr = is_prime_u64(v);
        } else {
            pr = true;
            for (int j = 0; j < np; ++j) if (rc[j] == 0) { pr = false; break; }
            if (pr) pr = A355958_mr(cur, k);
        }
        if (pr) {
            uint64_t c2 = 0;
            for (int j = 0; j < L; ++j) {
                c2 += (uint64_t)S[j] + cur[j];
                S[j] = (uint32_t)c2;
                c2 >>= 32;
            }
            for (int j = 0; j < np; ++j) rs[j] = (rs[j] + rc[j]) % (uint32_t)sp[j];
            ++count;
            if (count == n) {
                free(cur);
                free(S);
                return (seq_t)i;
            }
        }
    }
}
