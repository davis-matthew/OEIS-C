/*
 * A370130  a(n) = A369669(A276086(n)), where A369669 is the greatest common
 *          divisor of the first and second arithmetic derivative of n, and
 *          A276086 is the primorial base exp-function.
 * https://oeis.org/A370130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = gcd(N', N'') with N = A276086(n) = prod prime(k+1)^d_k
 *            (d_k = primorial-base digits of n), N' = arithmetic derivative;
 *            a(0)=0
 * Range:     u8 n <= 15, u16 n <= 57, u32 n <= 1393, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 117 terms through n = 9706 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static uint64_t A370130_next_prime(uint64_t p)
{
    uint64_t c = p + 1;
    for (;; ++c) {
        bool ok = c >= 2;
        for (uint64_t d = 2; d * d <= c; ++d) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) return c;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    uint64_t pr[80];
    int ex[80];
    int cnt = 0;

    /* primorial base digits -> exponents of successive primes */
    uint64_t p = 2;
    idx_t r = n;
    while (r > 0) {
        uint64_t d = (uint64_t)(r % p);
        r /= p;
        if (d > 0) {
            pr[cnt] = p;
            ex[cnt] = (int)d;
            ++cnt;
        }
        p = A370130_next_prime(p);
    }

    /* N' = g * S, g = prod p^(e-1), S = sum e * rad / p */
    seq_t rad = 1;
    seq_t g = 1;
    for (int i = 0; i < cnt; ++i) {
        rad *= (seq_t)pr[i];
        for (int j = 1; j < ex[i]; ++j) g *= (seq_t)pr[i];
    }
    seq_t S = 0;
    for (int i = 0; i < cnt; ++i)
        S += (seq_t)ex[i] * (rad / (seq_t)pr[i]);
    seq_t d1 = g * S;

    /* factorization of d1: primes of N with exponent e-1, plus those of S */
    seq_t fp[160];
    int fe[160];
    int fc = 0;
    for (int i = 0; i < cnt; ++i) {
        if (ex[i] > 1) {
            fp[fc] = (seq_t)pr[i];
            fe[fc] = ex[i] - 1;
            ++fc;
        }
    }
    int base_fc = fc;
    seq_t t = S;
    seq_t q = 2;
    while (q * q <= t) {
        if (t % q == 0) {
            int e = 0;
            while (t % q == 0) { t /= q; ++e; }
            int found = -1;
            for (int i = 0; i < base_fc; ++i)
                if (fp[i] == q) { found = i; break; }
            if (found >= 0) fe[found] += e;
            else { fp[fc] = q; fe[fc] = e; ++fc; }
        }
        q += (q == 2) ? 1 : 2;
    }
    if (t > 1) {
        int found = -1;
        for (int i = 0; i < base_fc; ++i)
            if (fp[i] == t) { found = i; break; }
        if (found >= 0) fe[found] += 1;
        else { fp[fc] = t; fe[fc] = 1; ++fc; }
    }

    seq_t d2 = 0;
    for (int i = 0; i < fc; ++i)
        d2 += (seq_t)fe[i] * (d1 / fp[i]);

    return gcd(d1, d2);
}
