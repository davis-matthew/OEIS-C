/*
 * A285221  Indices of primes in A022567.
 * https://oeis.org/A285221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search over n in increasing order for which A022567(n) is prime.
 *            A022567(n) = Sum_{T_k = k*(k+1)/2 <= n} p(n - T_k), where p =
 *            A000041. Mod 2, Product (1+x^m)^2 = Product (1+x^(2m)) = Sum
 *            x^(2*g), g = k*(3k-1)/2 (k in Z), so A022567(n) is odd only when
 *            n = k*(3k-1) or k*(3k+1); otherwise it is ev...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-08)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

/* ---- partition numbers stored as base-2^32 bignums ---- */
static uint32_t *A285221_pool = NULL;
static size_t A285221_pused = 0, A285221_pcap = 0;
static size_t *A285221_off = NULL;
static uint32_t *A285221_len = NULL;
static idx_t A285221_pn = 0, A285221_ncap = 0;
static uint32_t *A285221_s1 = NULL, *A285221_s2 = NULL;
static size_t A285221_scap = 0;

static void A285221_scratch(size_t L)
{
    if (L > A285221_scap) {
        size_t c = A285221_scap ? A285221_scap : 64;
        while (c < L) c <<= 1;
        A285221_s1 = (uint32_t *)realloc(A285221_s1, c * sizeof(uint32_t));
        A285221_s2 = (uint32_t *)realloc(A285221_s2, c * sizeof(uint32_t));
        A285221_scap = c;
    }
}

static void A285221_add(uint32_t *acc, size_t L, const uint32_t *src, size_t ls)
{
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < ls; ++i) {
        uint64_t x = (uint64_t)acc[i] + src[i] + c;
        acc[i] = (uint32_t)x;
        c = x >> 32;
    }
    for (; c && i < L; ++i) {
        uint64_t x = (uint64_t)acc[i] + c;
        acc[i] = (uint32_t)x;
        c = x >> 32;
    }
}

static void A285221_ensure(idx_t N)
{
    if (A285221_off == NULL) {
        A285221_ncap = 1024;
        A285221_off = (size_t *)malloc(A285221_ncap * sizeof(size_t));
        A285221_len = (uint32_t *)malloc(A285221_ncap * sizeof(uint32_t));
        A285221_pcap = 1 << 16;
        A285221_pool = (uint32_t *)malloc(A285221_pcap * sizeof(uint32_t));
        A285221_pool[0] = 1;
        A285221_off[0] = 0;
        A285221_len[0] = 1;
        A285221_pused = 1;
        A285221_pn = 1;
    }
    while (A285221_pn <= N) {
        idx_t cur = A285221_pn;
        size_t L = (size_t)A285221_len[cur - 1] + 2;
        A285221_scratch(L);
        uint32_t *pos = A285221_s1, *neg = A285221_s2;
        memset(pos, 0, L * sizeof(uint32_t));
        memset(neg, 0, L * sizeof(uint32_t));
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > cur) break;
            uint32_t *acc = (k & 1) ? pos : neg;
            A285221_add(acc, L, A285221_pool + A285221_off[cur - g1], A285221_len[cur - g1]);
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= cur)
                A285221_add(acc, L, A285221_pool + A285221_off[cur - g2], A285221_len[cur - g2]);
        }
        int64_t br = 0;
        for (size_t i = 0; i < L; ++i) {
            int64_t x = (int64_t)pos[i] - (int64_t)neg[i] - br;
            br = 0;
            if (x < 0) { x += ((int64_t)1 << 32); br = 1; }
            pos[i] = (uint32_t)x;
        }
        size_t len = L;
        while (len > 1 && pos[len - 1] == 0) --len;
        if (cur >= A285221_ncap) {
            A285221_ncap *= 2;
            A285221_off = (size_t *)realloc(A285221_off, A285221_ncap * sizeof(size_t));
            A285221_len = (uint32_t *)realloc(A285221_len, A285221_ncap * sizeof(uint32_t));
        }
        if (A285221_pused + len > A285221_pcap) {
            while (A285221_pused + len > A285221_pcap) A285221_pcap *= 2;
            A285221_pool = (uint32_t *)realloc(A285221_pool, A285221_pcap * sizeof(uint32_t));
        }
        memcpy(A285221_pool + A285221_pused, pos, len * sizeof(uint32_t));
        A285221_off[cur] = A285221_pused;
        A285221_len[cur] = (uint32_t)len;
        A285221_pused += len;
        A285221_pn++;
    }
}

/* ---- Montgomery arithmetic for Miller-Rabin ---- */
static bool A285221_geq(const uint32_t *x, const uint32_t *m, size_t s)
{
    for (size_t i = s; i-- > 0;) {
        if (x[i] != m[i]) return x[i] > m[i];
    }
    return true;
}

static void A285221_sub(uint32_t *x, const uint32_t *m, size_t s)
{
    int64_t br = 0;
    for (size_t i = 0; i < s; ++i) {
        int64_t v = (int64_t)x[i] - (int64_t)m[i] - br;
        br = 0;
        if (v < 0) { v += ((int64_t)1 << 32); br = 1; }
        x[i] = (uint32_t)v;
    }
}

static void A285221_dbl(uint32_t *x, const uint32_t *m, size_t s)
{
    uint32_t carry = x[s - 1] >> 31;
    for (size_t i = s - 1; i > 0; --i)
        x[i] = (x[i] << 1) | (x[i - 1] >> 31);
    x[0] <<= 1;
    if (carry || A285221_geq(x, m, s)) A285221_sub(x, m, s);
}

static void A285221_mmul(const uint32_t *a, const uint32_t *b, const uint32_t *m,
                         uint32_t minv, size_t s, uint32_t *t, uint32_t *out)
{
    memset(t, 0, (s + 2) * sizeof(uint32_t));
    for (size_t i = 0; i < s; ++i) {
        uint64_t C = 0, x;
        uint32_t bi = b[i];
        for (size_t j = 0; j < s; ++j) {
            x = (uint64_t)t[j] + (uint64_t)a[j] * bi + C;
            t[j] = (uint32_t)x;
            C = x >> 32;
        }
        x = (uint64_t)t[s] + C;
        t[s] = (uint32_t)x;
        t[s + 1] = (uint32_t)(x >> 32);
        uint32_t u = t[0] * minv;
        x = (uint64_t)t[0] + (uint64_t)u * m[0];
        C = x >> 32;
        for (size_t j = 1; j < s; ++j) {
            x = (uint64_t)t[j] + (uint64_t)u * m[j] + C;
            t[j - 1] = (uint32_t)x;
            C = x >> 32;
        }
        x = (uint64_t)t[s] + C;
        t[s - 1] = (uint32_t)x;
        C = x >> 32;
        t[s] = t[s + 1] + (uint32_t)C;
        t[s + 1] = 0;
    }
    if (t[s] || A285221_geq(t, m, s)) A285221_sub(t, m, s);
    memcpy(out, t, s * sizeof(uint32_t));
}

static bool A285221_mr(const uint32_t *m, size_t s)
{
    static const uint32_t bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    uint32_t inv = m[0];
    for (int i = 0; i < 5; ++i) inv *= 2 - m[0] * inv;
    uint32_t minv = (uint32_t)0 - inv;
    uint32_t *oneM = (uint32_t *)calloc(s, sizeof(uint32_t));
    uint32_t *mone = (uint32_t *)calloc(s, sizeof(uint32_t));
    uint32_t *bM = (uint32_t *)calloc(s, sizeof(uint32_t));
    uint32_t *y = (uint32_t *)calloc(s, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(s + 2, sizeof(uint32_t));
    bool result = true;

    oneM[0] = 1;
    for (size_t i = 0; i < 32 * s; ++i) A285221_dbl(oneM, m, s);
    memcpy(mone, m, s * sizeof(uint32_t));
    A285221_sub(mone, oneM, s);

    size_t top = 32 * s - 1;
    while (!((m[top / 32] >> (top % 32)) & 1)) --top;
    size_t r = 1;
    while (!((m[r / 32] >> (r % 32)) & 1)) ++r;

    for (size_t bi = 0; bi < sizeof(bases) / sizeof(bases[0]) && result; ++bi) {
        memset(bM, 0, s * sizeof(uint32_t));
        bM[0] = bases[bi];
        for (size_t i = 0; i < 32 * s; ++i) A285221_dbl(bM, m, s);
        memcpy(y, oneM, s * sizeof(uint32_t));
        for (size_t bit = top + 1; bit-- > r;) {
            A285221_mmul(y, y, m, minv, s, t, y);
            if ((m[bit / 32] >> (bit % 32)) & 1)
                A285221_mmul(y, bM, m, minv, s, t, y);
        }
        if (memcmp(y, oneM, s * sizeof(uint32_t)) == 0 ||
            memcmp(y, mone, s * sizeof(uint32_t)) == 0)
            continue;
        bool ok = false;
        for (size_t i = 1; i < r; ++i) {
            A285221_mmul(y, y, m, minv, s, t, y);
            if (memcmp(y, mone, s * sizeof(uint32_t)) == 0) { ok = true; break; }
            if (memcmp(y, oneM, s * sizeof(uint32_t)) == 0) break;
        }
        if (!ok) result = false;
    }
    free(oneM); free(mone); free(bM); free(y); free(t);
    return result;
}

/* ---- primality of A022567(N) ---- */
static uint32_t A285221_sp[700];
static size_t A285221_nsp = 0;

static bool A285221_test(idx_t N)
{
    if (A285221_nsp == 0) {
        static unsigned char comp[5001];
        for (uint32_t i = 3; i <= 5000; i += 2) {
            if (comp[i]) continue;
            A285221_sp[A285221_nsp++] = i;
            for (uint32_t j = i * i; j <= 5000; j += 2 * i) comp[j] = 1;
        }
    }
    A285221_ensure(N);
    size_t L = (size_t)A285221_len[N] + 2;
    uint32_t *v = (uint32_t *)calloc(L, sizeof(uint32_t));
    for (idx_t k = 0; ; ++k) {
        idx_t t = k * (k + 1) / 2;
        if (t > N) break;
        A285221_add(v, L, A285221_pool + A285221_off[N - t], A285221_len[N - t]);
    }
    size_t s = L;
    while (s > 1 && v[s - 1] == 0) --s;
    bool res;
    if (s <= 2) {
        uint64_t x = (uint64_t)v[0] | ((s == 2) ? ((uint64_t)v[1] << 32) : 0);
        res = is_prime_u64(x);
    } else if (!(v[0] & 1)) {
        res = false;
    } else {
        res = true;
        for (size_t i = 0; i < A285221_nsp; ++i) {
            uint64_t p = A285221_sp[i], rem = 0;
            for (size_t j = s; j-- > 0;)
                rem = ((rem << 32) | v[j]) % p;
            if (rem == 0) { res = false; break; }
        }
        if (res) res = A285221_mr(v, s);
    }
    free(v);
    return res;
}

/* ---- candidate enumeration: 1, then k(3k-1), k(3k+1) for k = 1, 2, ... ---- */
static idx_t *A285221_found = NULL;
static size_t A285221_nf = 0, A285221_fcap = 0;
static int A285221_phase = -1;   /* -1: candidate 1; 0: k(3k-1); 1: k(3k+1) */
static idx_t A285221_k = 1;

static idx_t A285221_next(void)
{
    idx_t c;
    if (A285221_phase == -1) {
        A285221_phase = 0;
        return 1;
    }
    if (A285221_phase == 0) {
        c = A285221_k * (3 * A285221_k - 1);
        A285221_phase = 1;
    } else {
        c = A285221_k * (3 * A285221_k + 1);
        A285221_phase = 0;
        A285221_k++;
    }
    return c;
}

seq_t a(idx_t n)
{
    while (A285221_nf < (size_t)n) {
        idx_t c = A285221_next();
        if (A285221_test(c)) {
            if (A285221_nf == A285221_fcap) {
                A285221_fcap = A285221_fcap ? A285221_fcap * 2 : 64;
                A285221_found = (idx_t *)realloc(A285221_found, A285221_fcap * sizeof(idx_t));
            }
            A285221_found[A285221_nf++] = c;
        }
    }
    return (seq_t)A285221_found[n - 1];
}
