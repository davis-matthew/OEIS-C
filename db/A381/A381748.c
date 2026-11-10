/*
 * A381748  a(n) is the number of primes (counted with multiplicity) in row n
 *          of A051599.
 * https://oeis.org/A381748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes in row n of A051599, where row n is
 *            built by Pascal's rule with ends prime(n+1); each entry is
 *            tested by trial division (primes < 2^16) followed by
 *            Miller-Rabin on multi-limb integers
 * Range:     u8 n <= 4999, u16 n <= 4999, u32 n <= 4999, u64 n <= 4999,
 *            u128 n <= 4999, big any n
 * Validated: standard, pass, 95 terms through n = 4805 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

typedef unsigned __int128 A381748_u128;

static uint32_t *A381748_pr = NULL;
static size_t A381748_npr = 0;
static uint32_t *A381748_gmod = NULL;
static size_t *A381748_gstart = NULL;
static size_t A381748_ngrp = 0;

static void A381748_primes(size_t need)
{
    if (A381748_pr && A381748_npr >= need) return;
    size_t lim = 65536;
    for (;;) {
        char *c = (char *)calloc(lim, 1);
        if (!c) abort();
        size_t cnt = 0;
        for (size_t i = 2; i < lim; ++i) {
            if (c[i]) continue;
            ++cnt;
            for (size_t j = i * i; j < lim; j += i) c[j] = 1;
        }
        if (cnt >= need) {
            free(A381748_pr);
            A381748_pr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
            if (!A381748_pr) abort();
            A381748_npr = 0;
            for (size_t i = 2; i < lim; ++i)
                if (!c[i]) A381748_pr[A381748_npr++] = (uint32_t)i;
            free(c);
            break;
        }
        free(c);
        lim *= 2;
    }
    if (!A381748_gmod) {
        /* group odd primes < 65536 into products < 2^32 */
        A381748_gmod = (uint32_t *)malloc(7000 * sizeof(uint32_t));
        A381748_gstart = (size_t *)malloc(7001 * sizeof(size_t));
        if (!A381748_gmod || !A381748_gstart) abort();
        size_t i = 1;
        A381748_ngrp = 0;
        while (i < A381748_npr && A381748_pr[i] < 65536) {
            uint64_t m = 1;
            A381748_gstart[A381748_ngrp] = i;
            while (i < A381748_npr && A381748_pr[i] < 65536 &&
                   m * A381748_pr[i] < ((uint64_t)1 << 32)) {
                m *= A381748_pr[i];
                ++i;
            }
            A381748_gmod[A381748_ngrp++] = (uint32_t)m;
        }
        A381748_gstart[A381748_ngrp] = i;
    }
}

static bool A381748_geq(const uint64_t *x, const uint64_t *y, size_t s)
{
    for (size_t i = s; i-- > 0;) {
        if (x[i] != y[i]) return x[i] > y[i];
    }
    return true;
}

static void A381748_sub(uint64_t *x, const uint64_t *y, size_t s)
{
    uint64_t br = 0;
    for (size_t i = 0; i < s; ++i) {
        uint64_t xi = x[i], yi = y[i];
        uint64_t d = xi - yi - br;
        br = (xi < yi) || (xi - yi < br);
        x[i] = d;
    }
}

/* x = 2x mod N, x < N */
static void A381748_dbl(uint64_t *x, const uint64_t *N, size_t s)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < s; ++i) {
        uint64_t v = x[i];
        x[i] = (v << 1) | carry;
        carry = v >> 63;
    }
    if (carry || A381748_geq(x, N, s)) A381748_sub(x, N, s);
}

static void A381748_mont(uint64_t *out, const uint64_t *a, const uint64_t *b,
                         const uint64_t *N, uint64_t ninv, size_t s, uint64_t *t)
{
    memset(t, 0, (s + 2) * sizeof(uint64_t));
    for (size_t i = 0; i < s; ++i) {
        uint64_t C = 0;
        A381748_u128 z;
        uint64_t bi = b[i];
        for (size_t j = 0; j < s; ++j) {
            z = (A381748_u128)a[j] * bi + t[j] + C;
            t[j] = (uint64_t)z;
            C = (uint64_t)(z >> 64);
        }
        z = (A381748_u128)t[s] + C;
        t[s] = (uint64_t)z;
        t[s + 1] = (uint64_t)(z >> 64);
        uint64_t m = t[0] * ninv;
        z = (A381748_u128)m * N[0] + t[0];
        C = (uint64_t)(z >> 64);
        for (size_t j = 1; j < s; ++j) {
            z = (A381748_u128)m * N[j] + t[j] + C;
            t[j - 1] = (uint64_t)z;
            C = (uint64_t)(z >> 64);
        }
        z = (A381748_u128)t[s] + C;
        t[s - 1] = (uint64_t)z;
        t[s] = t[s + 1] + (uint64_t)(z >> 64);
    }
    if (t[s] || A381748_geq(t, N, s)) A381748_sub(t, N, s);
    memcpy(out, t, s * sizeof(uint64_t));
}

static bool A381748_isprime(const uint64_t *N, size_t s)
{
    if (s == 1) return is_prime_u64(N[0]);
    if (!(N[0] & 1)) return false;
    /* trial division */
    for (size_t g = 0; g < A381748_ngrp; ++g) {
        uint64_t M = A381748_gmod[g], r = 0;
        for (size_t i = s; i-- > 0;) {
            r = ((r << 32) | (N[i] >> 32)) % M;
            r = ((r << 32) | (N[i] & 0xffffffffULL)) % M;
        }
        for (size_t k = A381748_gstart[g]; k < A381748_gstart[g + 1]; ++k)
            if (r % A381748_pr[k] == 0) return false;
    }
    /* Miller-Rabin */
    uint64_t *buf = (uint64_t *)malloc((5 * s + 2) * sizeof(uint64_t));
    if (!buf) abort();
    uint64_t *one = buf, *mone = buf + s, *bm = buf + 2 * s, *y = buf + 3 * s,
             *t = buf + 4 * s;
    uint64_t inv = N[0];
    for (int i = 0; i < 6; ++i) inv *= 2 - N[0] * inv;
    uint64_t ninv = (uint64_t)0 - inv;
    /* d = N-1 = 2^sh * odd */
    size_t sh = 0;
    {
        uint64_t low = N[0] - 1;
        if (low == 0) {
            size_t i = 1;
            while (N[i] == 0) ++i;
            sh = 64 * i;
            uint64_t v = N[i];
            while (!(v & 1)) { v >>= 1; ++sh; }
        } else {
            uint64_t v = low;
            while (!(v & 1)) { v >>= 1; ++sh; }
        }
    }
    size_t topbit = 64 * s - 1;
    while (!((N[topbit / 64] >> (topbit % 64)) & 1)) --topbit;
    memset(one, 0, s * sizeof(uint64_t));
    one[0] = 1;
    for (size_t i = 0; i < 64 * s; ++i) A381748_dbl(one, N, s);
    memcpy(mone, N, s * sizeof(uint64_t));
    A381748_sub(mone, one, s);
    static const uint32_t bases[6] = {2, 3, 5, 7, 11, 13};
    bool result = true;
    for (int bi = 0; bi < 6 && result; ++bi) {
        uint32_t b = bases[bi];
        memset(bm, 0, s * sizeof(uint64_t));
        bm[0] = b;
        for (size_t i = 0; i < 64 * s; ++i) A381748_dbl(bm, N, s);
        memcpy(y, bm, s * sizeof(uint64_t));
        /* bits of N-1 from topbit-1 down to sh (bits of N and N-1 agree for i>=1
           above the lowest set bit of N-1, and sh>=1) */
        for (size_t i = topbit; i-- > sh;) {
            A381748_mont(y, y, y, N, ninv, s, t);
            uint64_t bit = (N[i / 64] >> (i % 64)) & 1;
            if (bit) {
                if (b == 2) A381748_dbl(y, N, s);
                else A381748_mont(y, y, bm, N, ninv, s, t);
            }
        }
        if (memcmp(y, one, s * sizeof(uint64_t)) == 0 ||
            memcmp(y, mone, s * sizeof(uint64_t)) == 0)
            continue;
        bool ok = false;
        for (size_t r = 1; r < sh; ++r) {
            A381748_mont(y, y, y, N, ninv, s, t);
            if (memcmp(y, mone, s * sizeof(uint64_t)) == 0) { ok = true; break; }
            if (memcmp(y, one, s * sizeof(uint64_t)) == 0) break;
        }
        if (!ok) result = false;
    }
    free(buf);
    return result;
}

seq_t a(idx_t n)
{
    static uint64_t *rows = NULL;
    static size_t S = 0, caprows = 0;
    static size_t cur = 0;
    static bool have = false;
    static int32_t *cc = NULL;
    static size_t ccap = 0;

    if (n == 0) return (seq_t)1;
    size_t N = (size_t)n;
    if (N >= ccap) {
        size_t nc = N + N / 2 + 16;
        cc = (int32_t *)realloc(cc, nc * sizeof(int32_t));
        if (!cc) abort();
        for (size_t i = ccap; i < nc; ++i) cc[i] = -1;
        ccap = nc;
    }
    if (cc[N] >= 0) return (seq_t)cc[N];

    A381748_primes(N + 2);

    if (N >= caprows) {
        free(rows);
        caprows = N + N / 4 + 16;
        S = caprows / 64 + 2;
        rows = (uint64_t *)calloc(caprows * S, sizeof(uint64_t));
        if (!rows) abort();
        have = false;
    }
    if (!have || N < cur) {
        memset(rows, 0, S * sizeof(uint64_t));
        rows[0] = 2;
        cur = 0;
        have = true;
    }
    while (cur < N) {
        size_t k = cur + 1;
        uint64_t p = A381748_pr[k];
        size_t used = k / 64 + 2;
        if (used > S) used = S;
        for (size_t j = k - 1; j >= 1; --j) {
            uint64_t *dst = rows + j * S;
            const uint64_t *src = rows + (j - 1) * S;
            uint64_t carry = 0;
            for (size_t l = 0; l < used; ++l) {
                uint64_t x = dst[l] + carry;
                uint64_t c1 = x < carry;
                uint64_t z = x + src[l];
                carry = c1 + (z < x);
                dst[l] = z;
            }
        }
        memset(rows + k * S, 0, S * sizeof(uint64_t));
        rows[k * S] = p;
        rows[0] = p;
        cur = k;
    }

    int32_t c = 2;
    for (size_t j = 1; 2 * j <= N; ++j) {
        const uint64_t *x = rows + j * S;
        size_t len = S;
        while (len > 1 && x[len - 1] == 0) --len;
        if (A381748_isprime(x, len)) c += (2 * j == N) ? 1 : 2;
    }
    cc[N] = c;
    return (seq_t)c;
}
