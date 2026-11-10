/*
 * A373764  Odd composite numbers k such that A086615(k-1) == 2*(-1)^((k-1)/2)
 *          (mod k).
 * https://oeis.org/A373764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   search over odd composite k. A086615(k-1) = Sum_{j>=0}
 *            C(k,2j+1)*A000108(j) (A086615 is the partial sum of the Motzkin
 *            numbers). For each prime p | k with M = k/p, Lucas' theorem
 *            gives A086615(k-1) == 2*(-1)^((p-1)/2) * W(M) (mod p), where
 *            W(M) = Sum_{i<M} A002426(i). A002426 (the central trinomi...
 * Range:     u8 none, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A373764_inv = NULL, *A373764_T = NULL, *A373764_pre = NULL;
static size_t A373764_inv_cap = 0, A373764_T_cap = 0, A373764_pre_cap = 0;
static uint64_t *A373764_u = NULL, *A373764_iu = NULL, *A373764_vv = NULL;
static size_t A373764_u_cap = 0, A373764_iu_cap = 0, A373764_vv_cap = 0;

static void A373764_grow(uint64_t **b, size_t *cap, size_t n)
{
    if (n > *cap) {
        *b = (uint64_t *)realloc(*b, n * sizeof(uint64_t));
        *cap = n;
    }
}

static uint64_t A373764_invmod(uint64_t a, uint64_t m)
{
    long long t = 0, nt = 1, r = (long long)m, nr = (long long)(a % m);
    while (nr) {
        long long q = r / nr, tmp;
        tmp = t - q * nt; t = nt; nt = tmp;
        tmp = r - q * nr; r = nr; nr = tmp;
    }
    if (t < 0) t += (long long)m;
    return (uint64_t)t;
}

/* test W(M) == (-1)^((M-1)/2) (mod p), W(M) = sum_{i<M} T_i */
static int A373764_filter(uint64_t p, uint64_t M)
{
    uint64_t lim = (M < p - 1) ? M : p - 1;
    size_t sz = (size_t)lim + 2;
    A373764_grow(&A373764_inv, &A373764_inv_cap, sz);
    A373764_grow(&A373764_T, &A373764_T_cap, sz);
    A373764_grow(&A373764_pre, &A373764_pre_cap, sz);
    uint64_t *inv = A373764_inv, *T = A373764_T, *pre = A373764_pre;
    inv[1] = 1;
    for (uint64_t i = 2; i <= lim; i++)
        inv[i] = (p - (p / i) * inv[p % i] % p) % p;
    T[0] = 1 % p;
    T[1] = 1 % p;
    for (uint64_t n = 2; n <= lim; n++) {
        uint64_t s = ((2 * n - 1) % p) * T[n - 1] % p + ((3 * (n - 1)) % p) * T[n - 2] % p;
        T[n] = s % p * inv[n] % p;
    }
    pre[0] = 0;
    for (uint64_t x = 0; x <= lim; x++) pre[x + 1] = (pre[x] + T[x]) % p;

    uint64_t d[70];
    int r = 0;
    uint64_t m = M;
    while (m) { d[r++] = m % p; m /= p; }
    uint64_t Ptot = (lim == p - 1) ? pre[p] : 0;
    uint64_t pw[70];
    pw[0] = 1 % p;
    for (int i = 1; i < r; i++) pw[i] = pw[i - 1] * Ptot % p;
    uint64_t res = 0, pref = 1 % p;
    for (int pos = r - 1; pos >= 0; pos--) {
        res = (res + pref * pre[d[pos]] % p * pw[pos]) % p;
        if (pos > 0) pref = pref * T[d[pos]] % p;
    }
    uint64_t target = (((M - 1) / 2) % 2 == 0) ? 1 % p : p - 1;
    return res == target;
}

/* exact check of sum_j C(k,2j+1)Cat(j) == 2(-1)^((k-1)/2) (mod p^e) */
static int A373764_full(uint64_t k, uint64_t p, int e)
{
    uint64_t q = 1, pp[70];
    pp[0] = 1;
    for (int i = 1; i <= e; i++) { q *= p; pp[i] = q; }
    size_t sz = (size_t)k + 1;
    A373764_grow(&A373764_u, &A373764_u_cap, sz);
    A373764_grow(&A373764_iu, &A373764_iu_cap, sz);
    A373764_grow(&A373764_vv, &A373764_vv_cap, sz);
    uint64_t *u = A373764_u, *iu = A373764_iu, *vv = A373764_vv;
    u[0] = 1 % q; vv[0] = 0;
    for (uint64_t n = 1; n <= k; n++) {
        uint64_t x = n, c = 0;
        while (x % p == 0) { x /= p; c++; }
        u[n] = u[n - 1] * (x % q) % q;
        vv[n] = vv[n - 1] + c;
    }
    iu[k] = A373764_invmod(u[k], q);
    for (uint64_t n = k; n >= 1; n--) {
        uint64_t x = n;
        while (x % p == 0) x /= p;
        iu[n - 1] = iu[n] * (x % q) % q;
    }
    uint64_t S = 0;
    for (uint64_t j = 0; 2 * j + 1 <= k; j++) {
        uint64_t a = 2 * j + 1;
        long long val = (long long)vv[k] - (long long)vv[a] - (long long)vv[k - a]
                      + (long long)vv[2 * j] - (long long)vv[j] - (long long)vv[j + 1];
        if (val >= e) continue;
        uint64_t t = u[k] * iu[a] % q;
        t = t * iu[k - a] % q;
        t = t * u[2 * j] % q;
        t = t * iu[j] % q;
        t = t * iu[j + 1] % q;
        t = t * pp[val] % q;
        S = (S + t) % q;
    }
    uint64_t target = (((k - 1) / 2) % 2 == 0) ? 2 % q : (q - 2) % q;
    return S == target;
}

static int A373764_check(uint64_t k)
{
    uint64_t pr[64];
    int ex[64], cnt = 0;
    uint64_t m = k;
    for (uint64_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) {
            int c = 0;
            while (m % d == 0) { m /= d; c++; }
            pr[cnt] = d; ex[cnt] = c; cnt++;
        }
    }
    if (m > 1) { pr[cnt] = m; ex[cnt] = 1; cnt++; }
    if (cnt == 1 && ex[0] == 1) return 0; /* prime */
    for (int i = cnt - 1; i >= 0; i--)
        if (!A373764_filter(pr[i], k / pr[i])) return 0;
    for (int i = 0; i < cnt; i++)
        if (ex[i] >= 2 && !A373764_full(k, pr[i], ex[i])) return 0;
    return 1;
}

static uint64_t A373764_terms[64];
static size_t A373764_count = 0;
static uint64_t A373764_next = 9;

seq_t a(idx_t n)
{
    while (A373764_count < n && A373764_count < 64) {
        uint64_t k = A373764_next;
        A373764_next += 2;
        if (A373764_check(k)) A373764_terms[A373764_count++] = k;
    }
    return (seq_t)A373764_terms[n - 1];
}
