/*
 * A248142  Least positive integer m such that m + n divides A(m) + A(n),
 *          where A(.) is given by A005259.
 * https://oeis.org/A248142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = least m >= 1 with (m+n) | A005259(m) +
 *            A005259(n). Candidates are filtered prime by prime: A005259 has
 *            the Lucas property A(p*q+r) == A(q)*A(r) (mod p), and A(d) ==
 *            A(p-1-d) (mod p) for 0 <= d < p. If m+n is squarefree, passing
 *            every prime factor settles it. Otherwise A(m) mod (m+n...
 * Range:     u8 n <= 3, u16 n <= 1358, u32 n <= 2853, u64 n <= 2853,
 *            u128 n <= 2853, big any n
 * Validated: standard, pass, 76 terms through n = 2619 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t A248142_inv(uint64_t x, uint64_t M) {
    int64_t r0 = (int64_t)M, r1 = (int64_t)(x % M), s0 = 0, s1 = 1;
    while (r1 != 0) {
        int64_t q = r0 / r1, t;
        t = r0 - q * r1; r0 = r1; r1 = t;
        t = s0 - q * s1; s0 = s1; s1 = t;
    }
    if (s0 < 0) s0 += (int64_t)M;
    return (uint64_t)s0;
}

/* Apery number A005259(N) modulo M (M >= 2), exact for composite M */
static uint64_t A248142_apery_mod(uint64_t N, uint64_t M) {
    uint64_t ps[16];
    int ex[16];
    int np = 0;
    uint64_t r = M;
    for (uint64_t p = 2; p * p <= r; p++) {
        if (r % p == 0) {
            ps[np] = p; ex[np] = 0; np++;
            while (r % p == 0) r /= p;
        }
    }
    if (r > 1) { ps[np] = r; ex[np] = 0; np++; }

    uint64_t u = 1 % M;
    uint64_t sum = 1 % M;
    for (uint64_t k = 0; k < N; k++) {
        uint64_t f[4];
        int sg[4];
        f[0] = N - k;       sg[0] = 1;
        f[1] = N + k + 1;   sg[1] = 1;
        f[2] = k + 1;       sg[2] = -1;
        f[3] = k + 1;       sg[3] = -1;
        for (int j = 0; j < 4; j++) {
            uint64_t x = f[j];
            for (int i = 0; i < np; i++) {
                while (x % ps[i] == 0) { x /= ps[i]; ex[i] += sg[j]; }
            }
            x %= M;
            if (sg[j] > 0) u = u * x % M;
            else u = u * A248142_inv(x, M) % M;
        }
        uint64_t t = u;
        for (int i = 0; i < np && t != 0; i++) {
            for (int e = 0; e < ex[i] && t != 0; e++) t = t * (ps[i] % M) % M;
        }
        sum = (sum + t * t) % M;
    }
    return sum;
}

/* ---- cached exact Apery numbers as base-2^32 limb arrays ---- */
static uint32_t **A248142_BG = NULL;
static size_t *A248142_BL = NULL;
static size_t A248142_BN = 0, A248142_BCAP = 0;

static size_t A248142_mul_small(uint32_t *x, size_t len, uint32_t f) {
    uint64_t carry = 0;
    for (size_t i = 0; i < len; i++) {
        uint64_t v = (uint64_t)x[i] * f + carry;
        x[i] = (uint32_t)v;
        carry = v >> 32;
    }
    if (carry) x[len++] = (uint32_t)carry;
    return len;
}

static size_t A248142_div_small(uint32_t *x, size_t len, uint32_t d) {
    uint64_t rem = 0;
    for (size_t i = len; i-- > 0;) {
        uint64_t cur = (rem << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    while (len > 1 && x[len - 1] == 0) len--;
    return len;
}

static uint64_t A248142_big_mod(const uint32_t *x, size_t len, uint64_t p) {
    uint64_t r = 0;
    for (size_t i = len; i-- > 0;) r = ((r << 32) | x[i]) % p;
    return r;
}

static void A248142_ensure_big(uint64_t upto) {
    size_t need = (size_t)upto + 1;
    if (need <= A248142_BN) return;
    if (need > A248142_BCAP) {
        size_t nc = A248142_BCAP * 2;
        if (nc < need) nc = need;
        if (nc < 16) nc = 16;
        A248142_BG = (uint32_t **)realloc(A248142_BG, nc * sizeof(uint32_t *));
        A248142_BL = (size_t *)realloc(A248142_BL, nc * sizeof(size_t));
        A248142_BCAP = nc;
    }
    while (A248142_BN < need) {
        size_t j = A248142_BN;
        if (j < 2) {
            uint32_t *b = (uint32_t *)malloc(sizeof(uint32_t));
            b[0] = (j == 0) ? 1u : 5u;
            A248142_BG[j] = b; A248142_BL[j] = 1;
        } else {
            size_t L1 = A248142_BL[j - 1], L2 = A248142_BL[j - 2];
            uint32_t *T1 = (uint32_t *)malloc((L1 + 4) * sizeof(uint32_t));
            uint32_t *T2 = (uint32_t *)malloc((L2 + 4) * sizeof(uint32_t));
            memcpy(T1, A248142_BG[j - 1], L1 * sizeof(uint32_t));
            memcpy(T2, A248142_BG[j - 2], L2 * sizeof(uint32_t));
            uint64_t jj = (uint64_t)j;
            size_t l1 = L1, l2 = L2;
            l1 = A248142_mul_small(T1, l1, (uint32_t)(2 * jj - 1));
            l1 = A248142_mul_small(T1, l1, (uint32_t)(17 * jj * jj - 17 * jj + 5));
            for (int r = 0; r < 3; r++) l2 = A248142_mul_small(T2, l2, (uint32_t)(jj - 1));
            int64_t borrow = 0;
            for (size_t i = 0; i < l1; i++) {
                int64_t v = (int64_t)T1[i] - borrow - (i < l2 ? (int64_t)T2[i] : 0);
                if (v < 0) { v += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
                T1[i] = (uint32_t)v;
            }
            while (l1 > 1 && T1[l1 - 1] == 0) l1--;
            for (int r = 0; r < 3; r++) l1 = A248142_div_small(T1, l1, (uint32_t)jj);
            free(T2);
            A248142_BG[j] = (uint32_t *)realloc(T1, l1 * sizeof(uint32_t));
            A248142_BL[j] = l1;
        }
        A248142_BN++;
    }
}

/* A005259(d) mod prime p, 0 <= d < p */
static uint64_t A248142_digit_mod(uint64_t d, uint64_t p) {
    uint64_t dd = d;
    if (p - 1 - d < dd) dd = p - 1 - d;
    if (dd < A248142_BN) return A248142_big_mod(A248142_BG[dd], A248142_BL[dd], p);
    uint64_t num = 1, den = 1, S = 1;
    for (uint64_t k = 0; k < dd; k++) {
        num = num * ((dd - k) % p) % p;
        num = num * ((dd + k + 1) % p) % p;
        uint64_t f = (k + 1) % p;
        f = f * f % p;
        den = den * f % p;
        S = (S * f % p * f % p + num * num % p) % p;
    }
    uint64_t iv = A248142_inv(den * den % p, p);
    return S * iv % p;
}

/* A005259(x) mod prime p via Lucas property */
static uint64_t A248142_apery_mod_p(uint64_t x, uint64_t p) {
    uint64_t r = 1 % p;
    while (x > 0 && r != 0) {
        r = r * A248142_digit_mod(x % p, p) % p;
        x /= p;
    }
    return r;
}

/* ---- smallest prime factor sieve, grown on demand ---- */
static uint32_t *A248142_SPF = NULL;
static uint64_t A248142_SPFN = 0;

static void A248142_ensure_spf(uint64_t M) {
    if (M < A248142_SPFN) return;
    uint64_t N = 2 * M;
    if (N < 65536) N = 65536;
    free(A248142_SPF);
    A248142_SPF = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    for (uint64_t i = 2; i <= N; i++) {
        if (A248142_SPF[i] == 0) {
            for (uint64_t j = i; j <= N; j += i)
                if (A248142_SPF[j] == 0) A248142_SPF[j] = (uint32_t)i;
        }
    }
    A248142_SPFN = N + 1;
}

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    A248142_ensure_big(nn < 64 ? 64 : nn);
    for (uint64_t m = 1;; m++) {
        uint64_t M = m + nn;
        A248142_ensure_spf(M);
        uint64_t ps[20];
        int np = 0;
        int squarefree = 1;
        uint64_t r = M;
        while (r > 1) {
            uint64_t p = A248142_SPF[r];
            int e = 0;
            while (r % p == 0) { r /= p; e++; }
            if (e > 1) squarefree = 0;
            ps[np++] = p;
        }
        int ok = 1;
        for (int i = 0; i < np && ok; i++) {
            uint64_t p = ps[i];
            uint64_t s = (A248142_apery_mod_p(m, p) + A248142_apery_mod_p(nn, p)) % p;
            if (s != 0) ok = 0;
        }
        if (!ok) continue;
        if (squarefree) return (seq_t)m;
        uint64_t an = A248142_big_mod(A248142_BG[nn], A248142_BL[nn], M);
        if ((A248142_apery_mod(m, M) + an) % M == 0) return (seq_t)m;
    }
}
