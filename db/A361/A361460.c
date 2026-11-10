/*
 * A361460  a(n) = 1 if A135504(n+1) = 2 * A135504(n), otherwise 0.
 * https://oeis.org/A361460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if (n+1) divides A135504(n), else 0; with A135504(1)=1,
 *            A135504(k) = A135504(k-1)*(1 + k/gcd(A135504(k-1),k)). Computed
 *            by tracking the prime-exponent vector of A135504 (all primes <=
 *            n+1)
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 136 terms through n = 9340 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint32_t N = (uint32_t)n + 1;
    uint32_t *spf = (uint32_t *)malloc((size_t)(N + 1) * sizeof(uint32_t));
    uint32_t *ex  = (uint32_t *)calloc((size_t)(N + 1), sizeof(uint32_t));
    if (!spf || !ex) { free(spf); free(ex); return (seq_t)0; }
    for (uint32_t i = 0; i <= N; ++i) spf[i] = 0;
    for (uint32_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            for (uint32_t k = i; k <= N; k += i)
                if (spf[k] == 0) spf[k] = i;
        }
    }
    for (uint32_t j = 2; j <= (uint32_t)n; ++j) {
        /* g = gcd(x, j) */
        uint32_t g = 1, m = j;
        while (m > 1) {
            uint32_t p = spf[m], e = 0;
            while (m % p == 0) { m /= p; ++e; }
            uint32_t k = ex[p] < e ? ex[p] : e;
            while (k--) g *= p;
        }
        uint32_t r = 1 + j / g;
        while (r > 1) {
            uint32_t p = spf[r];
            while (r % p == 0) { r /= p; ex[p]++; }
        }
    }
    int ok = 1;
    uint32_t m = N;
    while (m > 1) {
        uint32_t p = spf[m], e = 0;
        while (m % p == 0) { m /= p; ++e; }
        if (ex[p] < e) { ok = 0; break; }
    }
    free(spf);
    free(ex);
    return (seq_t)(ok ? 1 : 0);
}
