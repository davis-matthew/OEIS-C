/*
 * A362186  a(n) is the least number k such that the equation A323410(x) = k
 *          has exactly n solutions, or -1 if no such k exists.
 * https://oeis.org/A362186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with exactly n solutions x of A323410(x) = k
 *            (A323410(x) = x - prod_{p^e||x}(p^e-1)); computed by
 *            sieve/enumeration, x = m*q with q the largest unitary
 *            prime-power factor
 * Range:     u8 n <= 18, u16 n <= 948, u32 n <= 1292, u64 n <= 1292,
 *            u128 n <= 1292, big any n
 * Validated: standard, pass, 72 terms through n = 1205 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint32_t *a362186_cnt = NULL;
static uint64_t a362186_K = 0;

static void a362186_build(uint64_t K)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)K + 2, sizeof(uint32_t));
    uint32_t *cnt = (uint32_t *)calloc((size_t)K + 2, sizeof(uint32_t));
    uint32_t *pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)K + 2));
    size_t np = 0;
    uint64_t i, j;
    for (i = 2; i <= K; i++) {
        if (spf[i] == 0) {
            pr[np++] = (uint32_t)i;
            for (j = i; j <= K; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (uint64_t m = 2; m <= K; m++) {
        uint64_t phi = 1, t = m, pmax = 0;
        while (t > 1) {
            uint64_t p = spf[t], q = 1;
            while (t % p == 0) { t /= p; q *= p; }
            phi *= (q - 1);
            if (p > pmax) pmax = p;
        }
        uint64_t d = m - phi;
        /* first prime index with prime > pmax */
        size_t lo = 0, hi = np;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (pr[mid] > pmax) hi = mid; else lo = mid + 1;
        }
        for (size_t ip = lo; ip < np; ip++) {
            uint64_t p = pr[ip];
            if (p * d + phi > K) break;
            uint64_t q = p;
            while (q * d + phi <= K) {
                cnt[q * d + phi]++;
                q *= p;
            }
        }
    }
    free(spf);
    free(pr);
    if (a362186_cnt) free(a362186_cnt);
    a362186_cnt = cnt;
    a362186_K = K;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    uint64_t K = a362186_K ? a362186_K : 1024;
    for (;;) {
        if (a362186_K < K || a362186_cnt == NULL) a362186_build(K);
        for (uint64_t k = 2; k <= a362186_K; k++) {
            if ((idx_t)a362186_cnt[k] == n) return (seq_t)k;
        }
        if (a362186_K >= ((uint64_t)1 << 23)) break;
        K = a362186_K * 2;
    }
    return (seq_t)0;
}
