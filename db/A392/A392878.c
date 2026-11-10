/*
 * A392878  a(n) = A276086(A276085(n)), where A276085 and A276086 are
 *          primorial base log and exp-functions.
 * https://oeis.org/A392878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A276086(A276085(n)); A276085 is fully additive with
 *            A276085(p) = A034386(p)/p, so its primorial-base digits are the
 *            exponents of n's prime factors (with carries), and A276086 reads
 *            those digits as exponents of successive primes
 * Range:     u8 n <= 256, u16 n <= 12000, u32 n <= 12000, u64 n <= 12000,
 *            u128 n <= 12000, big any n
 * Validated: standard, pass, 99 terms through n = 9052 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    uint64_t fp[64];
    uint64_t fe[64];
    size_t fc = 0;

    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            fp[fc] = p; fe[fc] = e; ++fc;
        }
    }
    if (m > 1) { fp[fc] = m; fe[fc] = 1; ++fc; }

    if (fc == 0)
        return (seq_t)1;

    uint64_t maxp = fp[fc - 1];

    /* primes list: all primes <= maxp, plus 80 more */
    size_t cap = 256, cnt = 0;
    uint64_t *pr = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!pr) abort();
    size_t extra = 0;
    for (uint64_t c = 2; ; ++c) {
        bool isp = true;
        for (size_t i = 0; i < cnt; ++i) {
            if (pr[i] * pr[i] > c) break;
            if (c % pr[i] == 0) { isp = false; break; }
        }
        if (!isp) continue;
        if (cnt == cap) {
            cap *= 2;
            pr = (uint64_t *)realloc(pr, cap * sizeof(uint64_t));
            if (!pr) abort();
        }
        pr[cnt++] = c;
        if (c > maxp) {
            if (++extra >= 80) break;
        }
    }

    uint64_t *d = (uint64_t *)calloc(cnt, sizeof(uint64_t));
    if (!d) abort();

    for (size_t i = 0; i < fc; ++i) {
        for (size_t k = 0; k < cnt; ++k) {
            if (pr[k] == fp[i]) { d[k] += fe[i]; break; }
        }
    }

    /* carry in primorial base */
    for (size_t k = 0; k + 1 < cnt; ++k) {
        if (d[k] >= pr[k]) {
            d[k + 1] += d[k] / pr[k];
            d[k] %= pr[k];
        }
    }

    seq_t result = 1;
    for (size_t k = 0; k < cnt; ++k) {
        for (uint64_t j = 0; j < d[k]; ++j)
            result *= (seq_t)pr[k];
    }

    free(pr);
    free(d);
    return result;
}
