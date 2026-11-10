/*
 * A374132  The 2-adic valuation of A276085(n), where A276085 is the primorial
 *          base log-function.
 * https://oeis.org/A374132
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007814(A276085(n)); A276085(n) = Sum_{p^e || n}
 *            e*(product of primes < p), evaluated mod 2^128 (sieve), then
 *            2-adic valuation taken
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 123 terms through n = 9731 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    typedef unsigned __int128 u128;
    uint64_t m = (uint64_t)n;
    uint64_t fp[64];
    uint64_t fe[64];
    int fc = 0;
    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            uint64_t e = 0;
            while (m % d == 0) { m /= d; ++e; }
            fp[fc] = d; fe[fc] = e; ++fc;
        }
    }
    if (m > 1) { fp[fc] = m; fe[fc] = 1; ++fc; }
    if (fc == 0) return (seq_t)0;

    uint64_t L = fp[fc - 1];
    unsigned char *comp = (unsigned char *)calloc((size_t)L + 1, 1);
    u128 prod = 1;
    u128 sum = 0;
    int idx = 0;
    for (uint64_t i = 2; i <= L; ++i) {
        if (comp[i]) continue;
        if (i <= L / i) {
            for (uint64_t j = i * i; j <= L; j += i) comp[j] = 1;
        }
        if (idx < fc && fp[idx] == i) {
            sum += (u128)fe[idx] * prod;
            ++idx;
        }
        prod *= (u128)i;
    }
    free(comp);

    if (sum == 0) return (seq_t)128;
    unsigned c = 0;
    while ((sum & 1) == 0) { sum >>= 1; ++c; }
    return (seq_t)c;
}
