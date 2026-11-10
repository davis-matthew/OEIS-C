/*
 * A361998  Indices of records in A361902.
 * https://oeis.org/A361998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = indices of records in A361902 (least k with m + F_k
 *            prime; 0 if m is prime). Search: scan m = 0, 1, 2, ...; m is a
 *            record when no k <= current record value makes m + F_k prime.
 *            Small cases use exact 64-bit tests, large ones use
 *            multiprecision Fibonacci numbers, a filter by small primes,
 *            an...
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../utils/is_prime_u64.h"

#define A361998_PLIM 3000
static uint32_t A361998_pr[600];
static int A361998_np = 0;

static void A361998_init_primes(void) {
    if (A361998_np) return;
    static unsigned char comp[A361998_PLIM + 1];
    for (int i = 2; i <= A361998_PLIM; i++) {
        if (comp[i]) continue;
        A361998_pr[A361998_np++] = (uint32_t)i;
        for (int j = i * i; j <= A361998_PLIM; j += i) comp[j] = 1;
    }
}

static uint32_t *A361998_Fa, *A361998_Fb, *A361998_N, *A361998_X, *A361998_Y,
                *A361998_T, *A361998_One, *A361998_Mone;
static int A361998_capl = 0;

static uint32_t *A361998_regrow(uint32_t *p, int oldc, int newc) {
    uint32_t *q = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(newc + 4));
    memset(q, 0, sizeof(uint32_t) * (size_t)(newc + 4));
    if (p) { memcpy(q, p, sizeof(uint32_t) * (size_t)(oldc + 4)); free(p); }
    return q;
}

static void A361998_ensure(int need) {
    if (need + 4 <= A361998_capl) return;
    int nc = A361998_capl ? A361998_capl : 64;
    while (nc < need + 4) nc *= 2;
    int oc = A361998_capl;
    A361998_Fa = A361998_regrow(A361998_Fa, oc, nc);
    A361998_Fb = A361998_regrow(A361998_Fb, oc, nc);
    A361998_N = A361998_regrow(A361998_N, oc, nc);
    A361998_X = A361998_regrow(A361998_X, oc, nc);
    A361998_Y = A361998_regrow(A361998_Y, oc, nc);
    A361998_T = A361998_regrow(A361998_T, oc, nc);
    A361998_One = A361998_regrow(A361998_One, oc, nc);
    A361998_Mone = A361998_regrow(A361998_Mone, oc, nc);
    A361998_capl = nc;
}

static int A361998_cmp(const uint32_t *a, const uint32_t *b, int s) {
    for (int i = s - 1; i >= 0; i--) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

static void A361998_sub(uint32_t *a, const uint32_t *b, int s) {
    uint64_t br = 0;
    for (int i = 0; i < s; i++) {
        uint64_t d = (uint64_t)a[i] - b[i] - br;
        a[i] = (uint32_t)d;
        br = (d >> 63) & 1;
    }
}

static void A361998_dbl_mod(uint32_t *x, const uint32_t *N, int s) {
    uint32_t carry = x[s - 1] >> 31;
    for (int i = s - 1; i > 0; i--) x[i] = (x[i] << 1) | (x[i - 1] >> 31);
    x[0] <<= 1;
    if (carry || A361998_cmp(x, N, s) >= 0) A361998_sub(x, N, s);
}

static void A361998_add_mod(uint32_t *x, const uint32_t *y, const uint32_t *N, int s) {
    uint64_t c = 0;
    for (int i = 0; i < s; i++) {
        c += (uint64_t)x[i] + y[i];
        x[i] = (uint32_t)c;
        c >>= 32;
    }
    if (c || A361998_cmp(x, N, s) >= 0) A361998_sub(x, N, s);
}

static void A361998_mont(uint32_t *r, const uint32_t *a, const uint32_t *b,
                         const uint32_t *N, int s, uint32_t ninv, uint32_t *t) {
    memset(t, 0, sizeof(uint32_t) * (size_t)(s + 2));
    for (int i = 0; i < s; i++) {
        uint64_t C = 0, cs;
        uint64_t bi = b[i];
        for (int j = 0; j < s; j++) {
            cs = (uint64_t)t[j] + (uint64_t)a[j] * bi + C;
            t[j] = (uint32_t)cs;
            C = cs >> 32;
        }
        cs = (uint64_t)t[s] + C;
        t[s] = (uint32_t)cs;
        t[s + 1] = (uint32_t)(cs >> 32);
        uint32_t mm = t[0] * ninv;
        cs = (uint64_t)t[0] + (uint64_t)mm * N[0];
        C = cs >> 32;
        for (int j = 1; j < s; j++) {
            cs = (uint64_t)t[j] + (uint64_t)mm * N[j] + C;
            t[j - 1] = (uint32_t)cs;
            C = cs >> 32;
        }
        cs = (uint64_t)t[s] + C;
        t[s - 1] = (uint32_t)cs;
        t[s] = t[s + 1] + (uint32_t)(cs >> 32);
    }
    if (t[s] || A361998_cmp(t, N, s) >= 0) A361998_sub(t, N, s);
    memcpy(r, t, sizeof(uint32_t) * (size_t)s);
}

/* strong probable-prime test of odd N (s limbs, top limb nonzero, N large) */
static bool A361998_sprp(const uint32_t *N, int s, int base) {
    uint32_t *X = A361998_X, *Y = A361998_Y, *T = A361998_T;
    uint32_t *One = A361998_One, *Mone = A361998_Mone;
    uint32_t inv = N[0];
    for (int i = 0; i < 5; i++) inv *= 2u - N[0] * inv;
    uint32_t ninv = (uint32_t)(0u - inv);

    memset(One, 0, sizeof(uint32_t) * (size_t)s);
    One[0] = 1;
    for (int i = 0; i < 32 * s; i++) A361998_dbl_mod(One, N, s);
    memcpy(Mone, N, sizeof(uint32_t) * (size_t)s);
    A361998_sub(Mone, One, s);

    int B = 32 * (s - 1);
    { uint32_t top = N[s - 1]; while (top) { B++; top >>= 1; } }
    int r = 1;
    while (!((N[r >> 5] >> (r & 31)) & 1u)) r++;

    memcpy(X, One, sizeof(uint32_t) * (size_t)s);
    for (int i = B - 1; i >= r; i--) {
        A361998_mont(X, X, X, N, s, ninv, T);
        if ((N[i >> 5] >> (i & 31)) & 1u) {
            if (base == 2) {
                A361998_dbl_mod(X, N, s);
            } else {
                memcpy(Y, X, sizeof(uint32_t) * (size_t)s);
                A361998_dbl_mod(X, N, s);
                A361998_add_mod(X, Y, N, s);
            }
        }
    }
    if (A361998_cmp(X, One, s) == 0 || A361998_cmp(X, Mone, s) == 0) return true;
    for (int j = 1; j < r; j++) {
        A361998_mont(X, X, X, N, s, ninv, T);
        if (A361998_cmp(X, Mone, s) == 0) return true;
        if (A361998_cmp(X, One, s) == 0) return false;
    }
    return false;
}

/* A361902(m) if it is <= cap; -2 if no k <= cap works; -1 if impossible */
static long long A361998_val(uint64_t m, long long cap) {
    if (is_prime_u64(m)) return 0;
    if (m % 1647030ULL == 14475ULL) return -1;
    uint64_t f0 = 0, f1 = 1;
    long long k;
    for (k = 1; k <= 90; k++) {
        if (k > cap) return -2;
        if (is_prime_u64(m + f1)) return k;
        uint64_t t = f0 + f1;
        f0 = f1;
        f1 = t;
    }
    /* now f0 = F_90, f1 = F_91 */
    A361998_init_primes();
    int np = A361998_np;
    uint32_t fa[600], fb[600], mp[600];
    for (int i = 0; i < np; i++) {
        uint32_t p = A361998_pr[i];
        fa[i] = (uint32_t)(f0 % p);
        fb[i] = (uint32_t)(f1 % p);
        mp[i] = (uint32_t)(m % p);
    }
    A361998_ensure(8);
    memset(A361998_Fa, 0, sizeof(uint32_t) * (size_t)A361998_capl);
    memset(A361998_Fb, 0, sizeof(uint32_t) * (size_t)A361998_capl);
    A361998_Fa[0] = (uint32_t)f0; A361998_Fa[1] = (uint32_t)(f0 >> 32);
    A361998_Fb[0] = (uint32_t)f1; A361998_Fb[1] = (uint32_t)(f1 >> 32);
    int len = 2;

    for (k = 91; k <= cap; k++) {
        bool comp = false;
        for (int i = 0; i < np; i++) {
            uint32_t p = A361998_pr[i];
            if ((mp[i] + fb[i]) % p == 0) { comp = true; break; }
        }
        if (!comp) {
            uint32_t *N = A361998_N;
            memcpy(N, A361998_Fb, sizeof(uint32_t) * (size_t)len);
            N[len] = 0; N[len + 1] = 0;
            uint64_t c = (uint64_t)N[0] + (uint32_t)m;
            N[0] = (uint32_t)c; c >>= 32;
            c += (uint64_t)N[1] + (uint32_t)(m >> 32);
            N[1] = (uint32_t)c; c >>= 32;
            for (int j = 2; c && j < len + 2; j++) {
                c += N[j];
                N[j] = (uint32_t)c;
                c >>= 32;
            }
            int s = len + 2;
            while (s > 1 && N[s - 1] == 0) s--;
            if (A361998_sprp(N, s, 2) && A361998_sprp(N, s, 3)) return k;
        }
        /* advance Fibonacci: (Fa, Fb) <- (Fb, Fa + Fb) */
        A361998_ensure(len + 4);
        {
            uint64_t c = 0;
            for (int j = 0; j < len; j++) {
                c += (uint64_t)A361998_Fa[j] + A361998_Fb[j];
                A361998_Fa[j] = (uint32_t)c;
                c >>= 32;
            }
            if (c) {
                A361998_Fa[len] = (uint32_t)c;
                A361998_Fb[len] = 0;
                len++;
            }
            uint32_t *tp = A361998_Fa;
            A361998_Fa = A361998_Fb;
            A361998_Fb = tp;
        }
        for (int i = 0; i < np; i++) {
            uint32_t p = A361998_pr[i];
            uint32_t t = (fa[i] + fb[i]) % p;
            fa[i] = fb[i];
            fb[i] = t;
        }
    }
    return -2;
}

seq_t a(idx_t n) {
    static uint64_t recs[256];
    static idx_t cnt = 0;
    static uint64_t next = 0;
    static long long best = -1;
    static bool best_exact = true;
    if (n < 1) return (seq_t)0;
    while (cnt < n && cnt < 256) {
        if (cnt == 0) {
            recs[cnt++] = 0;
            best_exact = false;
            next = 1;
            continue;
        }
        if (!best_exact) {
            best = A361998_val(recs[cnt - 1], LLONG_MAX);
            best_exact = true;
        }
        long long v = A361998_val(next, best);
        if (v == -2) {
            recs[cnt++] = next;
            best_exact = false;
        }
        next++;
    }
    return (seq_t)recs[n - 1];
}
