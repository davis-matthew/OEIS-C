/*
 * A343556  a(n) = denominator(max_{k=2..n}(A191898(n, k)/k)), n>=2.
 * https://oeis.org/A343556
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator(max_{k=2..n} T(n,k)/k), where T(n,k) =
 *            A191898(n,k) = Sum_{d | gcd(n,k)} d*mu(d) = Product_{p prime, p
 *            | gcd(n,k)} (1-p). Computed by scanning k = 2..n with exact
 *            rational comparison
 * Range:     u8 n <= 258, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 93 terms through n = 9616 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static long long A343556_gcd_ll(long long a, long long b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) { long long t = a % b; a = b; b = t; }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    long long N = (long long)n;

    /* distinct prime factors of n */
    long long pr[64];
    int np = 0;
    long long m = N;
    for (long long p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            pr[np++] = p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) pr[np++] = m;

    long long bestv = 0, bestk = 0;
    bool have = false;
    for (long long k = 2; k <= N; k++) {
        long long v = 1;
        for (int i = 0; i < np; i++)
            if (k % pr[i] == 0) v *= (1 - pr[i]);
        /* compare v/k > bestv/bestk  (k, bestk > 0) */
        if (!have || (__int128)v * bestk > (__int128)bestv * k) {
            bestv = v; bestk = k; have = true;
        }
    }
    long long g = A343556_gcd_ll(bestv, bestk);
    return (seq_t)(bestk / g);
}
