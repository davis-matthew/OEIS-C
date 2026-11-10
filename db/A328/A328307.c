/*
 * A328307  a(n) tells how many numbers m there are in range 0..n such that
 *          the k-th arithmetic derivative of A276086(m) is zero for some k >=
 *          0.
 * https://oeis.org/A328307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = a(n-1) + A328306(n). Computed by search: for
 *            each m, iterate the arithmetic derivative of A276086(m), keeping
 *            the number in factored form, until it reaches 1 (then 0,
 *            counted) or some p^p divides it (never 0)
 * Range:     u8 n <= 1613, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 101 terms through n = 8022 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef unsigned __int128 u128_328307;

#define MAXF_328307 160

static const unsigned small_primes_328307[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};

static unsigned tprimes_328307[200];
static int tcnt_328307 = 0;

static void init_tprimes_328307(void)
{
    if (tcnt_328307) return;
    for (unsigned c = 2; c < 1000; ++c) {
        bool ok = true;
        for (unsigned d = 2; d * d <= c; ++d) if (c % d == 0) { ok = false; break; }
        if (ok) tprimes_328307[tcnt_328307++] = c;
    }
}

static u128_328307 addm_328307(u128_328307 a, u128_328307 b, u128_328307 m)
{
    /* a, b < m */
    if (a >= m - b) return a - (m - b);
    return a + b;
}

static u128_328307 mulm_328307(u128_328307 a, u128_328307 b, u128_328307 m)
{
    a %= m; b %= m;
    if ((m >> 64) == 0) return (a * b) % m;
    /* b in c-bit chunks from the top, c = 128 - bits(m): r << c and a * chunk
       both stay below 2^128 (this took one bit of b per step, 128 steps, and
       timed out) */
    int bits = 128 - __builtin_clzll((unsigned long long)(m >> 64));   /* m >> 64 != 0 here */
    int c = 128 - bits;
    if (c < 1) c = 1;
    if (c > 63) c = 63;
    u128_328307 r = 0, mask = ((u128_328307)1 << c) - 1;
    int top = (b >> 64) ? 128 - __builtin_clzll((unsigned long long)(b >> 64))
            : b ? 64 - __builtin_clzll((unsigned long long)b) : 0;
    for (int s = (top + c - 1) / c * c - c; s >= 0; s -= c) {
        r = (r << c) % m;
        r = addm_328307(r, (a * ((b >> s) & mask)) % m, m);
    }
    return r;
}

static u128_328307 powm_328307(u128_328307 b, u128_328307 e, u128_328307 m)
{
    u128_328307 r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = mulm_328307(r, b, m);
        b = mulm_328307(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool isprime_328307(u128_328307 n)
{
    if (n < 2) return false;
    static const unsigned bases[12] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (int i = 0; i < 12; ++i) {
        if (n == bases[i]) return true;
        if (n % bases[i] == 0) return false;
    }
    u128_328307 d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    for (int i = 0; i < 12; ++i) {
        u128_328307 x = powm_328307(bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r) {
            x = mulm_328307(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static u128_328307 gcd128_328307(u128_328307 a, u128_328307 b)
{
    while (b) { u128_328307 t = a % b; a = b; b = t; }
    return a;
}

static u128_328307 rho_328307(u128_328307 n)
{
    if ((n & 1) == 0) return 2;
    for (u128_328307 c = 1;; ++c) {
        u128_328307 y = 2, x = 2, ys = 2, g = 1, q = 1;
        unsigned long r = 1;
        const unsigned long mstep = 64;
        do {
            x = y;
            for (unsigned long i = 0; i < r; ++i)
                y = addm_328307(mulm_328307(y, y, n), c % n, n);
            unsigned long k = 0;
            while (k < r && g == 1) {
                ys = y;
                unsigned long lim = (r - k < mstep) ? (r - k) : mstep;
                for (unsigned long i = 0; i < lim; ++i) {
                    y = addm_328307(mulm_328307(y, y, n), c % n, n);
                    u128_328307 df = x > y ? x - y : y - x;
                    q = mulm_328307(q, df, n);
                }
                g = gcd128_328307(q, n);
                k += mstep;
            }
            r <<= 1;
        } while (g == 1);
        if (g == n) {
            do {
                ys = addm_328307(mulm_328307(ys, ys, n), c % n, n);
                u128_328307 df = x > ys ? x - ys : ys - x;
                g = gcd128_328307(df, n);
            } while (g == 1);
        }
        if (g != n && g != 0) return g;
    }
}

typedef struct {
    u128_328307 pr[MAXF_328307];
    unsigned ex[MAXF_328307];
    int cnt;
    bool fail;
} fac_328307;

static void addf_328307(fac_328307 *f, u128_328307 p, unsigned e)
{
    for (int i = 0; i < f->cnt; ++i)
        if (f->pr[i] == p) { f->ex[i] += e; return; }
    if (f->cnt >= MAXF_328307) { f->fail = true; return; }
    f->pr[f->cnt] = p;
    f->ex[f->cnt] = e;
    f->cnt++;
}

static void factor_rec_328307(u128_328307 n, fac_328307 *f)
{
    if (n == 1 || f->fail) return;
    if (isprime_328307(n)) { addf_328307(f, n, 1); return; }
    u128_328307 d = rho_328307(n);
    factor_rec_328307(d, f);
    factor_rec_328307(n / d, f);
}

static void factor_into_328307(u128_328307 s, fac_328307 *f)
{
    for (int i = 0; i < tcnt_328307 && s > 1; ++i) {
        unsigned p = tprimes_328307[i];
        if ((u128_328307)p * p > s) break;
        if (s % p == 0) {
            unsigned e = 0;
            while (s % p == 0) { s /= p; ++e; }
            addf_328307(f, p, e);
        }
    }
    if (s > 1) {
        if (s < (u128_328307)1000 * 1000) addf_328307(f, s, 1);
        else factor_rec_328307(s, f);
    }
}

static bool good_328307(idx_t m)
{
    static fac_328307 bufA, bufB;
    fac_328307 *cur = &bufA, *nxt = &bufB;
    const u128_328307 MAXV = ~(u128_328307)0;

    cur->cnt = 0; cur->fail = false;
    uint64_t x = (uint64_t)m;
    for (int k = 0; x > 0 && k < 16; ++k) {
        uint64_t p = small_primes_328307[k];
        unsigned d = (unsigned)(x % p);
        x /= p;
        if (d) addf_328307(cur, p, d);
    }

    for (int it = 0; it < 5000; ++it) {
        if (cur->cnt == 0) return true;            /* value 1 -> derivative 0 */
        for (int i = 0; i < cur->cnt; ++i)
            if ((u128_328307)cur->ex[i] >= cur->pr[i]) return false;   /* p^p | v */
        u128_328307 rad = 1;
        for (int i = 0; i < cur->cnt; ++i) {
            if (rad > MAXV / cur->pr[i]) return false;
            rad *= cur->pr[i];
        }
        u128_328307 S = 0;
        for (int i = 0; i < cur->cnt; ++i) {
            u128_328307 t = rad / cur->pr[i];
            if (t > MAXV / cur->ex[i]) return false;
            t *= cur->ex[i];
            if (S + t < S) return false;
            S += t;
        }
        nxt->cnt = 0; nxt->fail = false;
        for (int i = 0; i < cur->cnt; ++i)
            if (cur->ex[i] >= 2) addf_328307(nxt, cur->pr[i], cur->ex[i] - 1);
        factor_into_328307(S, nxt);
        if (nxt->fail) return false;
        fac_328307 *tmp = cur; cur = nxt; nxt = tmp;
    }
    return false;
}

static idx_t *pref_328307 = NULL;   /* pref[m] = count of good in 0..m */
static idx_t pref_len_328307 = 0;
static idx_t pref_cap_328307 = 0;

seq_t a(idx_t n)
{
    init_tprimes_328307();
    while (pref_len_328307 <= n) {
        if (pref_len_328307 == pref_cap_328307) {
            idx_t nc = pref_cap_328307 ? pref_cap_328307 * 2 : 1024;
            while (nc <= n) nc *= 2;
            idx_t *np = (idx_t *)realloc(pref_328307, (size_t)nc * sizeof(idx_t));
            if (!np) abort();
            pref_328307 = np;
            pref_cap_328307 = nc;
        }
        idx_t m = pref_len_328307;
        idx_t prev = m ? pref_328307[m - 1] : 0;
        pref_328307[m] = prev + (good_328307(m) ? 1 : 0);
        ++pref_len_328307;
    }
    return (seq_t)pref_328307[n];
}
