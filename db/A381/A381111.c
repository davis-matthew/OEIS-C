/*
 * A381111  Least k such that A381110(k) = n.
 * https://oeis.org/A381111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(n) = least k with A381110(k) = n. A381110(k) = max
 *            over slopes p/q (q >= 1, p in {floor(q/phi), floor(q/phi)+1}) of
 *            1 + T, where T is the largest t <= k/q with f(k-j*q) = f(k) -
 *            j*p for all j <= t, and f(x) = floor(x/phi). The set of t where
 *            this holds is an interval starting at 0, so T...
 * Range:     u8 n <= 20, u16 n <= 322, u32 n <= 350, u64 n <= 350,
 *            u128 n <= 350, big any n
 * Validated: standard, pass, 70 terms through n = 308 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A381111_isqrt(uint64_t n)
{
    uint64_t x;
    if (n == 0) return 0;
    x = (uint64_t)__builtin_sqrt((double)n);
    while (x > 0 && x * x > n) x--;
    while ((x + 1) * (x + 1) <= n) x++;
    return x;
}

static uint64_t *A381111_f = NULL;
static uint64_t A381111_fn = 0;
static uint64_t A381111_fcap = 0;

static void A381111_ensure_f(uint64_t k)
{
    if (k < A381111_fn) return;
    if (k >= A381111_fcap) {
        uint64_t nc = A381111_fcap ? A381111_fcap : 1024;
        while (nc <= k) nc *= 2;
        A381111_f = (uint64_t *)realloc(A381111_f, nc * sizeof(uint64_t));
        A381111_fcap = nc;
    }
    while (A381111_fn <= k) {
        uint64_t kk = A381111_fn;
        uint64_t s = A381111_isqrt(5ULL * kk * kk);
        A381111_f[kk] = (s - kk) / 2ULL;
        A381111_fn++;
    }
}

static uint64_t A381110_a(uint64_t k)
{
    const double phinv = 0.61803398874989484820;
    uint64_t best = 1, q, pp;
    const uint64_t *f;
    uint64_t fk;
    double frac;
    if (k == 0) return 1;
    A381111_ensure_f(k);
    f = A381111_f;
    fk = f[k];
    frac = (double)k * phinv - (double)fk;
    if (frac < 0.0) frac = 0.0;
    if (frac >= 1.0) frac = 0.999999999999;
    for (q = 1; q <= k; q++) {
        uint64_t cap = k / q;
        if (cap + 1 <= best) break;
        for (pp = 0; pp < 2; pp++) {
            uint64_t p = f[q] + pp;
            double delta = (double)q * phinv - (double)p;
            double est;
            uint64_t T;
            if (delta > 0.0) est = frac / delta;
            else if (delta < 0.0) est = (1.0 - frac) / (-delta);
            else est = (double)cap;
            if (est < 0.0) est = 0.0;
            if (est >= (double)cap) T = cap;
            else T = (uint64_t)est;
            if (T > cap) T = cap;
            while (T > 0 && f[k - T * q] + T * p != fk) T--;
            while (T < cap && f[k - (T + 1) * q] + (T + 1) * p == fk) T++;
            if (T + 1 > best) best = T + 1;
        }
    }
    return best;
}

static uint64_t *A381111_first = NULL;   /* first k + 1, 0 = unseen */
static uint64_t A381111_cap = 0;
static uint64_t A381111_next = 0;

seq_t a(idx_t n)
{
    uint64_t target = (uint64_t)n;
    for (;;) {
        if (target < A381111_cap && A381111_first[target] != 0)
            return (seq_t)(A381111_first[target] - 1);
        {
            uint64_t k = A381111_next++;
            uint64_t v = A381110_a(k);
            if (v >= A381111_cap) {
                uint64_t nc = A381111_cap ? A381111_cap : 256;
                uint64_t i;
                while (nc <= v) nc *= 2;
                A381111_first = (uint64_t *)realloc(A381111_first, nc * sizeof(uint64_t));
                for (i = A381111_cap; i < nc; i++) A381111_first[i] = 0;
                A381111_cap = nc;
            }
            if (A381111_first[v] == 0) A381111_first[v] = k + 1;
        }
    }
}
