/*
 * A346601  Record values in A346599.
 * https://oeis.org/A346601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th record value of f(m) = least k with A344005(k) = m
 *            (m = 1, 2, 3, ...); computed by a search over divisors of
 *            m*(m+1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 195, u64 n <= 195,
 *            u128 n <= 195, big any n
 * Validated: standard, pass, 55 terms through n = 193 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 u128_t;

static uint64_t A346601_inv(uint64_t a, uint64_t mod)
{
    int64_t t = 0, nt = 1;
    int64_t r = (int64_t)mod, nr = (int64_t)(a % mod);
    while (nr != 0) {
        int64_t q = r / nr;
        int64_t tmp = t - q * nt; t = nt; nt = tmp;
        tmp = r - q * nr; r = nr; nr = tmp;
    }
    if (t < 0) t += (int64_t)mod;
    return (uint64_t)t;
}

static uint32_t *A346601_pr = 0;
static int A346601_npr = 0;

static void A346601_init(void)
{
    if (A346601_pr) return;
    const int L = 1 << 20;
    unsigned char *s = (unsigned char *)calloc(L + 1, 1);
    A346601_pr = (uint32_t *)malloc(sizeof(uint32_t) * 90000);
    for (int i = 2; i <= L; i++) {
        if (!s[i]) {
            A346601_pr[A346601_npr++] = (uint32_t)i;
            for (long long j = (long long)i * i; j <= L; j += i) s[j] = 1;
        }
    }
    free(s);
}

static int A346601_factor(uint64_t x, uint64_t *P, int *E, int cnt)
{
    for (int i = 0; i < A346601_npr; i++) {
        uint64_t p = A346601_pr[i];
        if (p * p > x) break;
        if (x % p == 0) {
            int e = 0;
            while (x % p == 0) { x /= p; e++; }
            P[cnt] = p; E[cnt] = e; cnt++;
        }
    }
    if (x > 1) {
        uint64_t p = (A346601_npr ? (uint64_t)A346601_pr[A346601_npr - 1] + 2 : 3);
        if (p % 2 == 0) p++;
        for (; p * p <= x; p += 2) {
            if (x % p == 0) {
                int e = 0;
                while (x % p == 0) { x /= p; e++; }
                P[cnt] = p; E[cnt] = e; cnt++;
            }
        }
        if (x > 1) { P[cnt] = x; E[cnt] = 1; cnt++; }
    }
    return cnt;
}

static uint64_t A346601_sums[1 << 16];

/* true iff A344005(n) == m, given n | m(m+1), n > m */
static bool A346601_ok(uint64_t n, uint64_t m, const uint64_t *P, int k)
{
    uint64_t qs[40];
    int kq = 0;
    uint64_t t = n;
    for (int i = 0; i < k; i++) {
        uint64_t p = P[i], q = 1;
        while (t % p == 0) { t /= p; q *= p; }
        if (q > 1) qs[kq++] = q;
    }
    if (kq > 16) return false;
    uint64_t Ei[16];
    for (int i = 0; i < kq; i++) {
        uint64_t M = n / qs[i];
        uint64_t iv = (qs[i] == 1) ? 0 : A346601_inv(M % qs[i], qs[i]);
        Ei[i] = (uint64_t)(((u128_t)M * iv) % n);
    }
    A346601_sums[0] = 0;
    uint64_t total = 1ULL << kq;
    for (uint64_t mask = 1; mask < total; mask++) {
        int low = __builtin_ctzll(mask);
        uint64_t s = A346601_sums[mask & (mask - 1)] + Ei[low];
        if (s >= n) s -= n;
        A346601_sums[mask] = s;
        uint64_t x = (s == 0) ? 0 : n - s;
        if (x > 0 && x < m) return false;
    }
    return true;
}

static int A346601_cmp(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}

static uint64_t A346601_rec[4096];
static int A346601_cnt = 0;
static uint64_t A346601_m = 0;
static uint64_t A346601_R = 0;

static void A346601_step(void)
{
    if (A346601_cnt == 0) {
        A346601_init();
        A346601_rec[A346601_cnt++] = 1;
        A346601_R = 1;
        A346601_m = 2;
        return;
    }
    uint64_t m = A346601_m++;
    uint64_t P[80];
    int E[80];
    int k = A346601_factor(m, P, E, 0);
    k = A346601_factor(m + 1, P, E, k);
    size_t nd = 1;
    for (int i = 0; i < k; i++) nd *= (size_t)(E[i] + 1);
    uint64_t *d = (uint64_t *)malloc(sizeof(uint64_t) * nd);
    size_t len = 1;
    d[0] = 1;
    for (int i = 0; i < k; i++) {
        size_t cur = len;
        uint64_t pw = 1;
        for (int e = 1; e <= E[i]; e++) {
            pw *= P[i];
            for (size_t j = 0; j < cur; j++) d[len++] = d[j] * pw;
        }
    }
    uint64_t R = A346601_R;
    bool found = false;
    for (size_t j = 0; j < len && !found; j++) {
        if (d[j] > m && d[j] <= R && A346601_ok(d[j], m, P, k)) found = true;
    }
    if (!found) {
        size_t c = 0;
        for (size_t j = 0; j < len; j++)
            if (d[j] > R && d[j] > m) d[c++] = d[j];
        qsort(d, c, sizeof(uint64_t), A346601_cmp);
        for (size_t j = 0; j < c; j++) {
            if (A346601_ok(d[j], m, P, k)) {
                A346601_rec[A346601_cnt++] = d[j];
                A346601_R = d[j];
                break;
            }
        }
    }
    free(d);
}

seq_t a(idx_t n)
{
    while ((idx_t)A346601_cnt < n && A346601_cnt < 4096) A346601_step();
    return (seq_t)A346601_rec[n - 1];
}
