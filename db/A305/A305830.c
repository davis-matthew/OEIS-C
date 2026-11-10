/*
 * A305830  Combined weight of the n-th FDH set-system. Factor n into distinct
 *          Fermi-Dirac primes (A050376), normalize by replacing every
 *          instance of the k-th Fermi-Dirac prime with k, then add up their
 *          FD-weights (A064547).
 * https://oeis.org/A305830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   n = f(s_1)*...*f(s_k) over distinct Fermi-Dirac primes f =
 *            A050376; a(n) = Sum_i A064547(s_i), where s_i = #{FD primes <=
 *            f(s_i)} = Sum_{j>=0} pi(floor(q^(1/2^j))). Computed by a sieve
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9755 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t fdw_isqrt(uint64_t x) {
    uint64_t r = 0, b = (uint64_t)1 << 62;
    while (b > x) b >>= 2;
    while (b) {
        if (x >= r + b) { x -= r + b; r = (r >> 1) + b; }
        else r >>= 1;
        b >>= 2;
    }
    return r;
}

static unsigned fdw_w(uint64_t m) {
    unsigned sum = 0;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            while (e) { sum += (unsigned)(e & 1); e >>= 1; }
        }
    }
    if (m > 1) sum += 1;
    return sum;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    uint64_t qs[130];
    size_t cnt = 0;
    uint64_t maxq = 1;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            uint64_t q = p;
            while (e) {
                if (e & 1) { qs[cnt++] = q; if (q > maxq) maxq = q; }
                e >>= 1;
                if (e) q = q * q;
            }
        }
    }
    if (t > 1) { qs[cnt++] = t; if (t > maxq) maxq = t; }
    if (cnt == 0) return (seq_t)0;

    size_t sz = (size_t)maxq + 1;
    unsigned char *comp = (unsigned char *)calloc(sz, 1);
    uint32_t *pi = (uint32_t *)malloc(sz * sizeof(uint32_t));
    for (uint64_t i = 2; i * i < sz; ++i)
        if (!comp[i])
            for (uint64_t j = i * i; j < sz; j += i) comp[j] = 1;
    uint32_t c = 0;
    pi[0] = 0;
    if (sz > 1) pi[1] = 0;
    for (size_t i = 2; i < sz; ++i) {
        if (!comp[i]) ++c;
        pi[i] = c;
    }
    seq_t total = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t x = qs[i], s = 0;
        while (x >= 2) { s += pi[x]; x = fdw_isqrt(x); }
        total += (seq_t)fdw_w(s);
    }
    free(comp);
    free(pi);
    return total;
}
