/*
 * A269848  a(1) = 1, a(2n) = A065090(1+a(n)), a(2n+1) =
 *          A000040(a(A064989(2n+1))).
 * https://oeis.org/A269848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(2n)=A065090(1+a(n)),
 *            a(2n+1)=A000040(a(A064989(2n+1))). Computed with Lucy_Hedgehog
 *            prime counting plus a short segmented sieve for the n-th prime
 *            and A065090
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 42, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-07)
 * Helpers:   prime_pi_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/prime_pi_u64.h"

static uint64_t *s269848_bp = NULL;
static size_t s269848_bpn = 0;
static uint64_t s269848_bplim = 0;

static uint64_t s269848_isqrt(uint64_t x)
{
    uint64_t r = (uint64_t)sqrtl((long double)x);
    while (r * r > x) r--;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

static void s269848_base_ensure(uint64_t lim)
{
    unsigned char *c;
    uint64_t i, j;
    size_t cnt = 0;
    if (s269848_bp && lim <= s269848_bplim) return;
    if (lim < 2 * s269848_bplim) lim = 2 * s269848_bplim;
    if (lim < 1000) lim = 1000;
    c = (unsigned char *)calloc((size_t)lim + 1, 1);
    if (!c) return;
    for (i = 2; i * i <= lim; i++)
        if (!c[i])
            for (j = i * i; j <= lim; j += i) c[j] = 1;
    for (i = 2; i <= lim; i++) if (!c[i]) cnt++;
    free(s269848_bp);
    s269848_bp = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    cnt = 0;
    for (i = 2; i <= lim; i++) if (!c[i]) s269848_bp[cnt++] = i;
    s269848_bpn = cnt;
    s269848_bplim = lim;
    free(c);
}

static uint64_t s269848_pi(uint64_t x)
{
    return prime_pi_u64(x);   /* db/utils/prime_pi_u64.h */
}

static long double s269848_li(long double x)
{
    long double lx = logl(x), term = 1, sum = 0, inner = 0, p2 = 1;
    int n;
    for (n = 1; n < 120; n++) {
        term *= lx / n;
        if (n > 1) p2 *= 2;
        if (n & 1) inner += 1.0L / (long double)(n);
        sum += ((n & 1) ? 1 : -1) * term / p2 * inner;
    }
    return 0.5772156649015329L + logl(lx) + sqrtl(x) * sum;
}

static long double s269848_R(long double x)
{
    return s269848_li(x) - 0.5L * s269848_li(sqrtl(x));
}

/* walk forward from x0 (count c at x0) until count reaches target.
   mode 0: count primes; mode 1: count non-odd-primes */
static uint64_t s269848_walk(uint64_t x0, uint64_t c, uint64_t target, int mode)
{
    enum { S = 1 << 18 };
    static unsigned char seg[S];
    uint64_t lo = x0 + 1;
    for (;;) {
        uint64_t hi = lo + S, i;
        size_t k;
        s269848_base_ensure(s269848_isqrt(hi) + 1);
        memset(seg, 1, S);
        for (k = 0; k < s269848_bpn; k++) {
            uint64_t p = s269848_bp[k], st, j;
            if (p * p >= hi) break;
            st = ((lo + p - 1) / p) * p;
            if (st < p * p) st = p * p;
            for (j = st - lo; j < S; j += p) seg[j] = 0;
        }
        for (i = 0; i < S; i++) {
            uint64_t v = lo + i;
            int hit = (mode == 0) ? (v >= 2 && seg[i]) : (v < 2 || !seg[i] || v == 2);
            if (hit) {
                c++;
                if (c == target) return v;
            }
        }
        lo = hi;
    }
}

static uint64_t s269848_nth_prime(uint64_t k)
{
    uint64_t x0, c;
    if (k < 1) return 0;
    if (k < 200) {
        x0 = 1; c = 0;
    } else {
        long double kk = (long double)k, x = kk * (logl(kk) + logl(logl(kk)) - 1.0L);
        uint64_t margin;
        int it;
        for (it = 0; it < 40; it++) {
            long double nx = x - (s269848_R(x) - kk) * logl(x);
            if (nx < 100) nx = 100;
            x = nx;
        }
        x0 = (uint64_t)x;
        margin = 3 * s269848_isqrt(x0) + 100;
        x0 = x0 > margin ? x0 - margin : 1;
        for (;;) {
            if (x0 <= 1) { x0 = 1; c = 0; break; }
            c = s269848_pi(x0);
            if (c < k) break;
            x0 = x0 > margin ? x0 - margin : 1;
        }
    }
    return s269848_walk(x0, c, k, 0);
}

/* A065090(m): m-th non-odd-prime */
static uint64_t s269848_A065090(uint64_t m)
{
    uint64_t x0, f0;
    if (m <= 2) return m;
    if (m < 200) {
        x0 = 1; f0 = 1;
    } else {
        long double mm = (long double)m, x = mm + mm / logl(mm);
        uint64_t margin;
        int it;
        for (it = 0; it < 40; it++) {
            long double g = x - s269848_R(x) + 1 - mm;
            long double nx = x - g / (1.0L - 1.0L / logl(x));
            if (nx < 100) nx = 100;
            x = nx;
        }
        x0 = (uint64_t)x;
        margin = 3 * s269848_isqrt(x0) + 100;
        x0 = x0 > margin ? x0 - margin : 1;
        for (;;) {
            if (x0 <= 1) { x0 = 1; f0 = 1; break; }
            f0 = x0 - s269848_pi(x0) + 1;
            if (f0 < m) break;
            x0 = x0 > margin ? x0 - margin : 1;
        }
    }
    return s269848_walk(x0, f0, m, 1);
}

static uint64_t s269848_prevprime(uint64_t p)
{
    uint64_t q = p - 1;
    for (;; q--) {
        uint64_t d;
        bool pr = q >= 2;
        for (d = 2; d * d <= q; d++) if (q % d == 0) { pr = false; break; }
        if (pr) return q;
    }
}

/* A064989 for odd n */
static uint64_t s269848_A064989(uint64_t n)
{
    uint64_t res = 1, d;
    for (d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            uint64_t pp = s269848_prevprime(d);
            while (n % d == 0) { n /= d; res *= pp; }
        }
    }
    if (n > 1) res *= s269848_prevprime(n);
    return res;
}

static uint64_t s269848_memo[2048];

static uint64_t s269848_a(uint64_t n)
{
    uint64_t r;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n < 2048 && s269848_memo[n]) return s269848_memo[n];
    if (n % 2 == 0)
        r = s269848_A065090(1 + s269848_a(n / 2));
    else
        r = s269848_nth_prime(s269848_a(s269848_A064989(n)));
    if (n < 2048) s269848_memo[n] = r;
    return r;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    return (seq_t)s269848_a((uint64_t)n);
}
