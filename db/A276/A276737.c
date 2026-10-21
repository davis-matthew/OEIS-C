/*
 * A276737  a(n) = denominator of Sum_{d|n} tau(d)/d.
 * https://oeis.org/A276737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Σ_{d|n} τ(d)/d, i.e. a(n) = n / gcd(n,
 *            Σ_{d|n} τ(d) · n/d)
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static void rec_divs(int idx, uint64_t d, seq_t tau, uint64_t n,
                     seq_t *num, int pcnt,
                     const int primes[], const int exps[])
{
    if (idx == pcnt) {
        seq_t term = tau * (seq_t)(n / d);
        *num += term;
        return;
    }
    const int p    = primes[idx];
    const int exp  = exps[idx];
    uint64_t nd = d;
    for (int e = 0; e <= exp; ++e) {
        seq_t ntau = tau * (seq_t)(e + 1);
        rec_divs(idx + 1, nd, ntau, n, num, pcnt, primes, exps);
        nd *= (uint64_t)p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */

    uint64_t m = (uint64_t)n;
    int primes[16], exps[16], pcnt = 0;

    /* factor the integer n */
    if ((m & 1ULL) == 0) {
        int cnt = 0;
        while ((m & 1ULL) == 0) { m >>= 1; ++cnt; }
        primes[pcnt] = 2;  exps[pcnt] = cnt;  ++pcnt;
    }
    for (uint64_t p = 3ULL; p * p <= m; p += 2ULL) {
        if (m % p != 0) continue;
        int cnt = 0;
        while (m % p == 0) { m /= p; ++cnt; }
        primes[pcnt] = (int)p;  exps[pcnt] = cnt;  ++pcnt;
    }
    if (m > 1) {
        primes[pcnt] = (int)m;  exps[pcnt] = 1;  ++pcnt;
    }

    /* compute N = Σ_{d|n} τ(d) · n/d  */
    seq_t num = 0;
    rec_divs(0, 1ULL, 1, (uint64_t)n, &num, pcnt, primes, exps);

    /* a(n) = n / gcd(n, N) */
    seq_t g = gcd(num, (seq_t)n);
    return (seq_t)n / g;
}
