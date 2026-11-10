/*
 * A375524  a(n) = denominator of Sum_{i=1..n} 1/A171397(i).
 * https://oeis.org/A375524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Sum_{i=1..n} 1/A171397(i). For each prime
 *            p, the p-adic part of the sum is tracked as X_p = Sum_{p | m_i}
 *            p^(E-v_p(m_i)) / (m_i/p^v_p(m_i)) (mod p^E), where p^E is the
 *            largest power of p <= max m_i. Then the exponent of p in a(n) is
 *            E - v_p(X_p), or 0 when X_p == 0. The nume...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 22, u64 n <= 48, u128 n <= 80,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 1962 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static uint64_t A375524_m(uint64_t n)
{
    uint64_t res = 0, power = 1, t = n;
    while (t != 0) {
        res += (t % 10) * power;
        power *= 11;
        t /= 10;
    }
    return res;
}

static long long A375524_inv(long long a, long long m)
{
    long long g = m, x = 0, x1 = 1, a1 = a % m;
    if (a1 < 0) a1 += m;
    while (a1 != 0) {
        long long q = g / a1, t;
        t = g - q * a1; g = a1; a1 = t;
        t = x - q * x1; x = x1; x1 = t;
    }
    x %= m;
    if (x < 0) x += m;
    return x;
}

static seq_t *A375524_memo = NULL;
static idx_t A375524_lim = 0;

static void A375524_build(idx_t limit)
{
    uint64_t maxm = 1, i, j;
    uint32_t *spf;
    long long *E, *P, *X;
    uint64_t *primes;
    size_t np = 0, k;

    for (i = 1; i <= limit; i++) {
        uint64_t m = A375524_m(i);
        if (m > maxm) maxm = m;
    }
    spf = (uint32_t *)calloc((size_t)maxm + 1, sizeof(uint32_t));
    E = (long long *)calloc((size_t)maxm + 1, sizeof(long long));
    P = (long long *)calloc((size_t)maxm + 1, sizeof(long long));
    X = (long long *)calloc((size_t)maxm + 1, sizeof(long long));
    primes = (uint64_t *)malloc(((size_t)maxm + 1) * sizeof(uint64_t));
    for (i = 2; i <= maxm; i++) {
        if (spf[i] == 0) {
            long long pe = (long long)i, e = 1;
            primes[np++] = i;
            for (j = i; j <= maxm; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
            while ((uint64_t)pe * i <= maxm) { pe *= (long long)i; e++; }
            E[i] = e;
            P[i] = pe;
        }
    }

    if (A375524_memo) mem_free(A375524_memo);
    A375524_memo = mem_new(seq_t, (size_t)limit + 1);

    for (i = 1; i <= limit; i++) {
        uint64_t m = A375524_m(i), r = m;
        seq_t den = 1;
        while (r > 1) {
            uint64_t p = spf[r];
            long long v = 0, mod = P[p], term, pw = 1;
            uint64_t mp = m;
            while (r % p == 0) { r /= p; v++; }
            for (j = 0; j < (uint64_t)v; j++) mp /= p;
            for (j = 0; j < (uint64_t)(E[p] - v); j++) pw *= (long long)p;
            term = (pw % mod) * A375524_inv((long long)(mp % (uint64_t)mod), mod) % mod;
            X[p] = (X[p] + term) % mod;
        }
        for (k = 0; k < np; k++) {
            uint64_t p = primes[k];
            long long x = X[p], v = 0, d;
            if (p > maxm) break;
            if (x == 0) continue;
            while (x % (long long)p == 0) { x /= (long long)p; v++; }
            d = E[p] - v;
            for (j = 0; j < (uint64_t)d; j++) den *= (seq_t)p;
        }
        A375524_memo[i] = den;
    }
    A375524_lim = limit;

    free(spf); free(E); free(P); free(X); free(primes);
}

seq_t a(idx_t n)
{
    if (n > A375524_lim) {
        idx_t lim = n * 2;
        if (lim < 2200) lim = 2200;
        A375524_build(lim);
    }
    return A375524_memo[n];
}
