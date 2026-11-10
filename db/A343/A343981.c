/*
 * A343981  a(n) is the least integer h such that there exists a Pythagorean
 *          triple whose hypotenuse is h and whose other legs z satisfy
 *          A176774(z) = n.
 * https://oeis.org/A343981
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   search. For k = 1, 2, ... take x = P(n,k) = ((n-2)k^2-(n-4)k)/2
 *            whose smallest polygonality A176774(x) is n. For each such x,
 *            list every Pythagorean triple with x as the larger leg by
 *            running over the divisors d of x^2 (y = (x^2/d-d)/2, h =
 *            (x^2/d+d)/2). Keep the triple if A176774(y) = n. Stop on...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 69 (2026-11-10)
 * Helpers:   isqrt_u128, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isqrt_u64.h"
#include "../utils/isqrt_u128.h"

typedef unsigned __int128 A343981_u128;

static uint64_t *A343981_pr = NULL;
static size_t A343981_npr = 0;

static void A343981_init_primes(void)
{
    if (A343981_pr) return;
    const size_t L = (size_t)1 << 20;
    unsigned char *c = (unsigned char *)calloc(L + 1, 1);
    size_t cnt = 0;
    for (size_t i = 2; i <= L; ++i) {
        if (!c[i]) { cnt++; for (size_t j = i * i; j <= L; j += i) c[j] = 1; }
    }
    A343981_pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    for (size_t i = 2; i <= L; ++i) if (!c[i]) A343981_pr[A343981_npr++] = i;
    free(c);
}

static void A343981_add_factor(uint64_t p, int e, uint64_t *P, int *E, int *cnt)
{
    for (int i = 0; i < *cnt; ++i) if (P[i] == p) { E[i] += e; return; }
    P[*cnt] = p; E[*cnt] = e; (*cnt)++;
}

static void A343981_factor(uint64_t v, uint64_t *P, int *E, int *cnt)
{
    for (size_t i = 0; i < A343981_npr && v > 1; ++i) {
        uint64_t p = A343981_pr[i];
        if (p * p > v) break;
        if (v % p == 0) {
            int e = 0;
            while (v % p == 0) { v /= p; e++; }
            A343981_add_factor(p, e, P, E, cnt);
        }
    }
    if (v > 1) {
        uint64_t p = A343981_pr[A343981_npr - 1] + 2;
        while (p * p <= v) {
            if (v % p == 0) {
                int e = 0;
                while (v % p == 0) { v /= p; e++; }
                A343981_add_factor(p, e, P, E, cnt);
            }
            p += 2;
        }
        if (v > 1) A343981_add_factor(v, 1, P, E, cnt);
    }
}

static A343981_u128 A343981_isqrt(A343981_u128 D)
{
    if ((D >> 64) == 0) return (A343981_u128)isqrt_u64((uint64_t)D);
    return isqrt_u128(D);
}

/* is x an m-gonal number (m >= 3, x >= 1)? */
static bool A343981_is_mgonal(A343981_u128 x, uint64_t m)
{
    A343981_u128 a2 = (A343981_u128)(m - 2);
    A343981_u128 D;
    if (m >= 4) { A343981_u128 b = m - 4; D = b * b + 8 * a2 * x; }
    else D = 1 + 8 * a2 * x;
    A343981_u128 s = A343981_isqrt(D);
    if (s * s != D) return false;
    A343981_u128 num = (m >= 4) ? s + (A343981_u128)(m - 4) : s - 1;
    return num % (2 * a2) == 0 && num >= 2 * a2;
}

static bool A343981_polyg_is(A343981_u128 x, uint64_t n)
{
    if (!A343981_is_mgonal(x, n)) return false;
    for (uint64_t m = 3; m < n; ++m)
        if (A343981_is_mgonal(x, m)) return false;
    return true;
}

typedef struct {
    uint64_t P[64];
    int E2[64];
    int cnt;
    A343981_u128 x, x2;
    uint64_t n;
    A343981_u128 best;
    bool have;
} A343981_ctx;

static void A343981_dfs(A343981_ctx *c, int i, A343981_u128 d)
{
    if (i == c->cnt) {
        A343981_u128 x = c->x;
        if (d >= x) return;
        if (d * (d + 2 * x) <= c->x2) return;     /* other leg would be >= x */
        A343981_u128 e = c->x2 / d;
        if (((e - d) & 1) != 0) return;
        A343981_u128 h = (e + d) / 2;
        if (c->have && h >= c->best) return;
        A343981_u128 y = (e - d) / 2;
        if (A343981_polyg_is(y, c->n)) { c->best = h; c->have = true; }
        return;
    }
    A343981_u128 dd = d;
    for (int k = 0; k <= c->E2[i]; ++k) {
        if (dd >= c->x) break;
        A343981_dfs(c, i + 1, dd);
        dd *= c->P[i];
    }
}

seq_t a(idx_t n)
{
    if (n < 3 || n == 4 || n == 6) return 0;
    A343981_init_primes();
    A343981_ctx c;
    c.n = (uint64_t)n;
    c.have = false;
    c.best = 0;
    for (uint64_t k = 1; ; ++k) {
        uint64_t t = (c.n - 2) * k + 4 - c.n;   /* (n-2)k - (n-4) */
        A343981_u128 x = (A343981_u128)k * t / 2;
        if (c.have && x >= c.best) break;
        if (!A343981_polyg_is(x, c.n)) continue;
        uint64_t P[64]; int E[64]; int cnt = 0;
        uint64_t kk = k, tt = t;
        if (kk % 2 == 0) kk /= 2; else tt /= 2;
        A343981_factor(kk, P, E, &cnt);
        A343981_factor(tt, P, E, &cnt);
        c.cnt = cnt;
        for (int i = 0; i < cnt; ++i) { c.P[i] = P[i]; c.E2[i] = 2 * E[i]; }
        c.x = x;
        c.x2 = x * x;
        A343981_dfs(&c, 0, 1);
    }
    A343981_u128 b = c.best;
    if ((b >> 64) == 0) return (seq_t)(uint64_t)b;
    seq_t r = (seq_t)(uint64_t)(b >> 64);
    r <<= 32; r <<= 32;
    r += (seq_t)(uint64_t)b;
    return r;
}
