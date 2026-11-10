/*
 * A316346  a(n) = A316297(n+1)/A316297(n).
 * https://oeis.org/A316346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A316297(n+1)/A316297(n) = (n+1) * den(H(n+1))/den(H(n)),
 *            with H(k) = Sum_{i=1..k} 1/i. Only primes p | (n+1) can change
 *            the denominator, so a(n) = Prod_{p^e || n+1} p^(e + d_p(n+1) -
 *            d_p(n)), where d_p(k) = max(0, -v_p(H(k))) is computed exactly
 *            by modular arithmetic
 * Range:     u8 n <= 15, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned long long A316346_inv(unsigned long long a, unsigned long long m)
{
    long long r0 = (long long)m, r1 = (long long)(a % m);
    long long s0 = 0, s1 = 1;
    while (r1 != 0) {
        long long q = r0 / r1, t;
        t = r0 - q * r1; r0 = r1; r1 = t;
        t = s0 - q * s1; s0 = s1; s1 = t;
    }
    s0 %= (long long)m;
    if (s0 < 0) s0 += (long long)m;
    return (unsigned long long)s0;
}

/* d_p(k) = p-adic valuation of the denominator of H(k) */
static unsigned long long A316346_dval(unsigned long long k, unsigned long long p)
{
    if (k == 0) return 0;
    unsigned long long pw = 1, e = 0;
    while (pw <= k / p) { pw *= p; e++; }       /* pw = p^e <= k */
    unsigned long long mod = pw * p;             /* p^(e+1) */
    unsigned long long S = 0;
    for (unsigned long long i = 1; i <= k; i++) {
        unsigned long long u = i, v = 0;
        while (u % p == 0) { u /= p; v++; }
        unsigned long long rp = 1;
        for (unsigned long long j = v; j < e; j++) rp *= p;   /* p^(e-v) */
        unsigned long long iv = A316346_inv(u % mod, mod);
        unsigned long long term =
            (unsigned long long)(((unsigned __int128)(rp % mod) * iv) % mod);
        S = (S + term) % mod;
    }
    if (S == 0) return 0;
    unsigned long long vs = 0;
    while (S % p == 0) { S /= p; vs++; }
    return vs < e ? e - vs : 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    unsigned long long m = (unsigned long long)n + 1;
    unsigned long long rest = m;
    seq_t res = 1;
    for (unsigned long long p = 2; rest > 1; p++) {
        if (p * p > rest) p = rest;
        if (rest % p != 0) continue;
        long long ex = 0;
        while (rest % p == 0) { rest /= p; ex++; }
        ex += (long long)A316346_dval(m, p) - (long long)A316346_dval(m - 1, p);
        while (ex > 0) { res *= (seq_t)p; ex--; }
    }
    return res;
}
