/*
 * A242770  Composite numbers of the form A242490(n) - 1.
 * https://oeis.org/A242770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search. For k = 2, 3, ... with p = prime(k): A242490(k) - 1 =
 *            p*m + 2, where m is the least odd number with m = 1 or lpf(m) >=
 *            p such that lpf(p*m+2) > p. a(n) is the n-th such value, taken
 *            in order of k, that is composite. A composite V = p*m+2 with
 *            lpf(V) > p has its least factor r in (p, sqrt(...
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

static unsigned char *A242770_comp = NULL;
static uint32_t *A242770_pr = NULL;
static size_t A242770_cnt = 0;
static uint64_t A242770_lim = 0;

static void A242770_resieve(uint64_t lim)
{
    unsigned char *s = (unsigned char *)calloc((size_t)lim + 1, 1);
    uint32_t *pr;
    size_t c = 0;
    uint64_t i, j;
    if (!s) abort();
    pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)lim / 2 + 16));
    if (!pr) abort();
    s[0] = s[1] = 1;
    for (i = 2; i <= lim; i++) {
        if (!s[i]) {
            pr[c++] = (uint32_t)i;
            for (j = i * i; j <= lim; j += i) s[j] = 1;
        }
    }
    free(A242770_comp);
    free(A242770_pr);
    A242770_comp = s;
    A242770_pr = pr;
    A242770_cnt = c;
    A242770_lim = lim;
}

static void A242770_ensure(uint64_t x)
{
    if (x > A242770_lim) {
        uint64_t nl = A242770_lim ? A242770_lim * 2 : ((uint64_t)1 << 21);
        while (nl < x) nl *= 2;
        A242770_resieve(nl);
    }
}

static void A242770_ensure_cnt(size_t c)
{
    while (A242770_cnt <= c)
        A242770_resieve(A242770_lim ? A242770_lim * 2 : ((uint64_t)1 << 21));
}

static uint64_t A242770_mulmod(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)((unsigned __int128)a * b % m);
}

static uint64_t A242770_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    b %= m;
    while (e) {
        if (e & 1) r = A242770_mulmod(r, b, m);
        b = A242770_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool A242770_isprime(uint64_t n)
{
    static const uint64_t bases[12] = {2,3,5,7,11,13,17,19,23,29,31,37};
    uint64_t d, x;
    int r = 0, i, j;
    if (n < 2) return false;
    for (i = 0; i < 12; i++) {
        if (n == bases[i]) return true;
        if (n % bases[i] == 0) return false;
    }
    d = n - 1;
    while ((d & 1) == 0) { d >>= 1; r++; }
    for (i = 0; i < 12; i++) {
        x = A242770_powmod(bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        for (j = 1; j < r; j++) {
            x = A242770_mulmod(x, x, n);
            if (x == n - 1) break;
        }
        if (j == r) return false;
    }
    return true;
}

static uint64_t A242770_isqrt(uint64_t v)
{
    uint64_t s = 0, b = (uint64_t)1 << 32;
    while (b) {
        uint64_t t = s + b;
        if ((unsigned __int128)t * t <= v) s = t;
        b >>= 1;
    }
    return s;
}

/* Is x free of prime factors < p ? (x >= 1) */
static bool A242770_rough(uint64_t x, uint64_t p)
{
    size_t i;
    A242770_ensure(A242770_isqrt(x) + 2);
    for (i = 0; i < A242770_cnt; i++) {
        uint64_t r = A242770_pr[i];
        if (r >= p || r * r > x) break;
        if (x % r == 0) return false;
    }
    return true;
}

/* For k >= 2: return A242490(k)-1 if it is composite, else 0. */
static uint64_t A242770_val(size_t k)
{
    uint64_t p, m;
    A242770_ensure_cnt(k + 1);
    p = A242770_pr[k - 1];
    if (A242770_isprime(p + 2)) return 0;           /* m = 1, value p+2 prime */
    for (m = p; ; m += 2) {
        uint64_t V, s;
        size_t i;
        bool ok = true;
        /* m must have lpf(m) >= p */
        if (m < p * p) {
            A242770_ensure(m + 1);
            if (A242770_comp[m]) continue;
        } else if (!A242770_rough(m, p)) continue;
        V = p * m + 2;
        /* quick filter by small primes below p */
        for (i = 0; i < 15 && i < A242770_cnt; i++) {
            uint64_t r = A242770_pr[i];
            if (r >= p) break;
            if (V % r == 0) { ok = false; break; }
        }
        if (!ok) continue;
        if (A242770_isprime(V)) return 0;           /* prime value: not a term */
        s = A242770_isqrt(V);
        A242770_ensure(s + 2);
        A242770_ensure_cnt(k + 1);
        for (i = k; i < A242770_cnt && (uint64_t)A242770_pr[i] <= s; i++) {
            uint64_t r = A242770_pr[i];
            if (V % r == 0) {
                if (A242770_rough(V / r, p)) return V;   /* lpf(V) = r > p */
                ok = false;
                break;
            }
        }
        /* no factor in (p, sqrt V] (or a smaller one exists): lpf(V) < p */
    }
}

seq_t a(idx_t n)
{
    static uint64_t *res = NULL;
    static size_t found = 0, cap = 0;
    static size_t nextk = 2;
    if (n < 1) return 0;
    while (found < (size_t)n) {
        uint64_t V = A242770_val(nextk++);
        if (V) {
            if (found + 1 >= cap) {
                cap = cap ? cap * 2 : 16;
                res = (uint64_t *)realloc(res, cap * sizeof(uint64_t));
                if (!res) abort();
            }
            res[++found] = V;
        }
    }
    return (seq_t)res[n];
}
