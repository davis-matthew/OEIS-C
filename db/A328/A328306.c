/*
 * A328306  a(n) = 1 if k-th arithmetic derivative of A276086(n) is zero for
 *          some k, otherwise 0.
 * https://oeis.org/A328306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A328308(A276086(n)): build m = A276086(n) from the
 *            primorial-base digits of n, then apply the arithmetic derivative
 *            repeatedly. Return 1 if m reaches 0 (a prime goes to 1, then to
 *            0). Return 0 if some p^p divides m, since then p^p divides every
 *            later derivative
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 8853 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef unsigned __int128 A328306_U;

#define A328306_LIMIT (((A328306_U)1) << 126)

/* ---------- 128-bit Montgomery arithmetic (odd modulus < 2^127) ---------- */
typedef struct {
    A328306_U N, ninv, one, r2;
} A328306_mont;

static void A328306_mul256(A328306_U a, A328306_U b, A328306_U *hi, A328306_U *lo)
{
    uint64_t a0 = (uint64_t)a, a1 = (uint64_t)(a >> 64);
    uint64_t b0 = (uint64_t)b, b1 = (uint64_t)(b >> 64);
    A328306_U p00 = (A328306_U)a0 * b0, p01 = (A328306_U)a0 * b1;
    A328306_U p10 = (A328306_U)a1 * b0, p11 = (A328306_U)a1 * b1;
    A328306_U mid = (p00 >> 64) + (uint64_t)p01 + (uint64_t)p10;
    *lo = (A328306_U)(uint64_t)p00 | (mid << 64);
    *hi = p11 + (p01 >> 64) + (p10 >> 64) + (mid >> 64);
}

static A328306_U A328306_redc(const A328306_mont *M, A328306_U hi, A328306_U lo)
{
    A328306_U m = lo * M->ninv;
    A328306_U mh, ml;
    A328306_mul256(m, M->N, &mh, &ml);
    A328306_U t = hi + mh + (lo != 0 ? 1 : 0);
    if (t >= M->N) t -= M->N;
    return t;
}

static A328306_U A328306_mmul(const A328306_mont *M, A328306_U a, A328306_U b)
{
    A328306_U hi, lo;
    A328306_mul256(a, b, &hi, &lo);
    return A328306_redc(M, hi, lo);
}

static void A328306_setup(A328306_mont *M, A328306_U N)
{
    A328306_U x = N;
    int i;
    M->N = N;
    for (i = 0; i < 7; i++) x *= (A328306_U)2 - N * x;
    M->ninv = (A328306_U)0 - x;
    M->one = ((A328306_U)0 - N) % N;
    {
        A328306_U r = M->one;
        for (i = 0; i < 128; i++) {
            r = r + r;
            if (r >= N) r -= N;
        }
        M->r2 = r;
    }
}

static A328306_U A328306_to_mont(const A328306_mont *M, A328306_U a)
{
    return A328306_mmul(M, a % M->N, M->r2);
}

static A328306_U A328306_mpow(const A328306_mont *M, A328306_U b, A328306_U e)
{
    A328306_U r = M->one;
    while (e) {
        if (e & 1) r = A328306_mmul(M, r, b);
        b = A328306_mmul(M, b, b);
        e >>= 1;
    }
    return r;
}

static A328306_U A328306_gcd(A328306_U a, A328306_U b)
{
    while (b) {
        A328306_U t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* n has no prime factor below 2000, n > 1 */
static bool A328306_is_prime(A328306_U n)
{
    static const unsigned bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
                                     41, 43, 47, 53, 59, 61, 67, 71};
    A328306_mont M;
    A328306_U d, minus;
    int s = 0;
    size_t i;
    if (n < (A328306_U)4000000) return true;
    if ((n & 1) == 0) return false;
    A328306_setup(&M, n);
    d = n - 1;
    while ((d & 1) == 0) { d >>= 1; s++; }
    minus = n - M.one;
    for (i = 0; i < sizeof(bases) / sizeof(bases[0]); i++) {
        A328306_U x = A328306_mpow(&M, A328306_to_mont(&M, (A328306_U)bases[i]), d);
        bool comp;
        int r;
        if (x == M.one || x == minus) continue;
        comp = true;
        for (r = 1; r < s; r++) {
            x = A328306_mmul(&M, x, x);
            if (x == minus) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* n odd composite, returns a nontrivial factor */
static A328306_U A328306_rho(A328306_U n)
{
    A328306_mont M;
    A328306_U cc;
    A328306_setup(&M, n);
    for (cc = 1;; cc++) {
        A328306_U c = A328306_to_mont(&M, cc);
        A328306_U y = A328306_to_mont(&M, cc + 1), x = y, ys = y, q = M.one, g = 1;
        A328306_U r = 1, k, i;
        const A328306_U m = 128;
        do {
            x = y;
            for (i = 0; i < r; i++) {
                y = A328306_mmul(&M, y, y) + c;
                if (y >= n) y -= n;
            }
            k = 0;
            do {
                A328306_U lim = (r - k < m) ? (r - k) : m;
                ys = y;
                for (i = 0; i < lim; i++) {
                    y = A328306_mmul(&M, y, y) + c;
                    if (y >= n) y -= n;
                    q = A328306_mmul(&M, q, x > y ? x - y : y - x);
                }
                g = A328306_gcd(q, n);
                k += m;
            } while (k < r && g == 1);
            r <<= 1;
        } while (g == 1);
        if (g == n) {
            do {
                ys = A328306_mmul(&M, ys, ys) + c;
                if (ys >= n) ys -= n;
                g = A328306_gcd(x > ys ? x - ys : ys - x, n);
            } while (g == 1);
        }
        if (g != n) return g;
    }
}

static void A328306_add_factor(A328306_U p, int e, A328306_U *ps, int *es, int *cnt)
{
    int i;
    for (i = 0; i < *cnt; i++)
        if (ps[i] == p) { es[i] += e; return; }
    ps[*cnt] = p;
    es[*cnt] = e;
    (*cnt)++;
}

static void A328306_factor_big(A328306_U n, A328306_U *ps, int *es, int *cnt)
{
    A328306_U d;
    if (n == 1) return;
    if (A328306_is_prime(n)) { A328306_add_factor(n, 1, ps, es, cnt); return; }
    d = A328306_rho(n);
    A328306_factor_big(d, ps, es, cnt);
    A328306_factor_big(n / d, ps, es, cnt);
}

static void A328306_factor(A328306_U m, A328306_U *ps, int *es, int *cnt)
{
    uint64_t d;
    *cnt = 0;
    for (d = 2; d < 2000; d += (d == 2 ? 1 : 2)) {
        if ((A328306_U)d * d > m) break;
        if (m % d == 0) {
            int e = 0;
            while (m % d == 0) { m /= d; e++; }
            A328306_add_factor((A328306_U)d, e, ps, es, cnt);
        }
    }
    if (m > 1) A328306_factor_big(m, ps, es, cnt);
}

static uint64_t A328306_next_prime(uint64_t p)
{
    for (;;) {
        uint64_t d;
        bool ok = true;
        p++;
        for (d = 2; d * d <= p; d++)
            if (p % d == 0) { ok = false; break; }
        if (ok) return p;
    }
}

seq_t a(idx_t n)
{
    A328306_U m = 1;
    A328306_U ps[130];
    int es[130];
    int cnt, step, i;
    idx_t rem = n;
    uint64_t p = 2;

    /* m = A276086(n) */
    while (rem > 0) {
        uint64_t dgt = (uint64_t)(rem % p), j;
        rem /= p;
        for (j = 0; j < dgt; j++) {
            if (m > A328306_LIMIT / p) return (seq_t)0; /* out of range */
            m *= p;
        }
        p = A328306_next_prime(p);
    }

    for (step = 0; step < 100000; step++) {
        int omega_total = 0;
        A328306_U total = 0;
        if (m <= 1) return (seq_t)1;
        A328306_factor(m, ps, es, &cnt);
        for (i = 0; i < cnt; i++) {
            if (ps[i] < 200 && (A328306_U)es[i] >= ps[i]) return (seq_t)0;
            omega_total += es[i];
        }
        if (omega_total == 1) return (seq_t)1; /* prime -> 1 -> 0 */
        for (i = 0; i < cnt; i++) {
            A328306_U q = m / ps[i];
            if (q > A328306_LIMIT / (A328306_U)es[i]) return (seq_t)0;
            q *= (A328306_U)es[i];
            if (total > A328306_LIMIT - q) return (seq_t)0;
            total += q;
        }
        m = total;
    }
    return (seq_t)0;
}
