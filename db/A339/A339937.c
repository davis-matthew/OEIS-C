/*
 * A339937  Numbers k such that k and k+1 are both coreful abundant numbers
 *          (A308053).
 * https://oeis.org/A339937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. k is coreful abundant iff its powerful part m satisfies
 *            csigma(m) > 2*m, and every multiple of such an m is coreful
 *            abundant too. Each pair (k, k+1) contains exactly one odd
 *            number. The code lists the odd powerful primitive coreful
 *            abundant numbers m, takes their odd multiples, and tests...
 * Range:     u8 none, u16 none, u32 n <= 205, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 923 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 A339937_u128;

static uint64_t *A339937_T = 0;
static size_t A339937_Tn = 0, A339937_Tcap = 0;
static uint64_t A339937_hi = 0;

static uint32_t *A339937_pr = 0;
static size_t A339937_np = 0;
static uint64_t A339937_plim = 0;

static uint64_t *A339937_prim = 0;
static size_t A339937_primn = 0, A339937_primcap = 0;

static uint64_t A339937_stp[64], A339937_stpw[64];
static A339937_u128 A339937_stS[64];

static uint64_t A339937_isqrt(uint64_t x)
{
    uint64_t r = 0, bit = (uint64_t)1 << 62;
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) { x -= r + bit; r = (r >> 1) + bit; }
        else r >>= 1;
        bit >>= 2;
    }
    return r;
}

static void A339937_ensure_primes(uint64_t need)
{
    if (need <= A339937_plim) return;
    uint64_t lim = A339937_plim * 2;
    if (lim < need) lim = need;
    if (lim < 4096) lim = 4096;
    unsigned char *s = (unsigned char *)calloc((size_t)lim + 1, 1);
    free(A339937_pr);
    A339937_pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)lim / 2 + 16));
    A339937_np = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (!s[i]) {
            A339937_pr[A339937_np++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= lim; j += i) s[j] = 1;
        }
    }
    free(s);
    A339937_plim = lim;
}

static void A339937_push(uint64_t **arr, size_t *n, size_t *cap, uint64_t v)
{
    if (*n == *cap) {
        *cap = *cap ? *cap * 2 : 1024;
        *arr = (uint64_t *)realloc(*arr, *cap * sizeof(uint64_t));
    }
    (*arr)[(*n)++] = v;
}

static bool A339937_primitive(int depth, A339937_u128 c2, uint64_t mm)
{
    for (int d = 0; d < depth; d++) {
        uint64_t p = A339937_stp[d], pw = A339937_stpw[d];
        A339937_u128 S = A339937_stS[d];
        A339937_u128 c;
        uint64_t m2;
        if (pw == p * p) { c = c2 / S; m2 = mm / pw; }
        else { c = c2 / S * (S - pw); m2 = mm / p; }
        if (c > (A339937_u128)2 * m2) return false;
    }
    return true;
}

static void A339937_dfs(size_t i, uint64_t m, A339937_u128 cs, int depth, uint64_t L)
{
    for (; i < A339937_np; i++) {
        uint64_t p = A339937_pr[i];
        if ((A339937_u128)m * p * p > L) break;
        uint64_t pw = p * p;
        A339937_u128 S = (A339937_u128)p + pw;
        for (;;) {
            uint64_t mm = m * pw;
            A339937_u128 c2 = cs * S;
            if (c2 > (A339937_u128)2 * mm) {
                if (A339937_primitive(depth, c2, mm))
                    A339937_push(&A339937_prim, &A339937_primn, &A339937_primcap, mm);
                break;
            }
            A339937_stp[depth] = p; A339937_stpw[depth] = pw; A339937_stS[depth] = S;
            A339937_dfs(i + 1, mm, c2, depth + 1, L);
            if ((A339937_u128)mm * p > L) break;
            pw *= p;
            S += pw;
        }
    }
}

static int A339937_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return u < v ? -1 : (u > v ? 1 : 0);
}

static bool A339937_test(uint64_t x, uint64_t c)
{
    if (x < 2) return false;
    A339937_u128 cs = 1;
    uint64_t r = x;
    for (size_t i = 0; i < A339937_np; i++) {
        uint64_t p = A339937_pr[i];
        if (p > c) break;
        if (p * p > r) break;
        if (r % p == 0) {
            uint64_t pw = 1;
            A339937_u128 S = 0;
            while (r % p == 0) { r /= p; pw *= p; S += pw; }
            cs *= S;
        }
    }
    if (r > 1) {
        uint64_t s = A339937_isqrt(r);
        if (s * s == r) cs *= (A339937_u128)s + (A339937_u128)s * s;
        else cs *= r;
    }
    return cs > (A339937_u128)2 * x;
}

static void A339937_block(uint64_t lo, uint64_t hi)
{
    uint64_t L = hi - 1;
    A339937_ensure_primes(A339937_isqrt(hi) + 2);
    A339937_primn = 0;
    A339937_dfs(1, 1, 1, 0, L);

    uint64_t *K = 0;
    size_t kn = 0, kcap = 0;
    for (size_t t = 0; t < A339937_primn; t++) {
        uint64_t m = A339937_prim[t];
        uint64_t j = (lo + m - 1) / m;
        if (j == 0) j = 1;
        if ((j & 1) == 0) j++;
        for (; (A339937_u128)m * j <= L; j += 2)
            A339937_push(&K, &kn, &kcap, m * j);
    }
    if (kn) qsort(K, kn, sizeof(uint64_t), A339937_cmp);

    uint64_t c = 1;
    while ((A339937_u128)c * c * c < (A339937_u128)hi + 2) c++;

    uint64_t last = 0;
    bool have = false;
    for (size_t t = 0; t < kn; t++) {
        uint64_t k = K[t];
        if (have && k == last) continue;
        have = true;
        last = k;
        if (A339937_test(k - 1, c))
            A339937_push(&A339937_T, &A339937_Tn, &A339937_Tcap, k - 1);
        if (A339937_test(k + 1, c))
            A339937_push(&A339937_T, &A339937_Tn, &A339937_Tcap, k);
    }
    free(K);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A339937_Tn < (size_t)n) {
        uint64_t lo = A339937_hi;
        uint64_t hi = lo ? lo * 2 : ((uint64_t)1 << 20);
        A339937_block(lo, hi);
        A339937_hi = hi;
    }
    return (seq_t)A339937_T[n - 1];
}
