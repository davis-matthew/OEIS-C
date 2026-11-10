/*
 * A337474  Number of prime shifts (x -> A003961(x)) needed before the result
 *          is deficient, when starting from x = A108951(n), the primorial
 *          inflation of n.
 * https://oeis.org/A337474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A336835(A108951(n)); A108951 maps n = Prod prime(i)^e_i
 *            to Prod (prime(i)#)^e_i. Computed by iterating the prime shift
 *            on exponent vectors until sigma(x)/x < 2
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9307 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    uint64_t m = (uint64_t)n;
    uint64_t fp[64]; unsigned fe[64]; int fc = 0;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            fp[fc] = p; fe[fc] = e; fc++;
        }
    }
    if (m > 1) { fp[fc] = m; fe[fc] = 1; fc++; }
    uint64_t lpf = fp[fc - 1];

    /* primes list */
    uint64_t limit = lpf * 2 + 4000;
    uint64_t *pr = 0; size_t M = 0;
    for (;;) {
        char *comp = (char *)calloc(limit + 1, 1);
        size_t cnt = 0;
        for (uint64_t i = 2; i <= limit; i++) {
            if (!comp[i]) {
                cnt++;
                for (uint64_t j = i * i; j <= limit; j += i) comp[j] = 1;
            }
        }
        size_t need = 0;
        for (uint64_t i = 2; i <= lpf; i++) if (!comp[i]) need++;
        need += 300;
        if (cnt >= need) {
            M = need;
            pr = (uint64_t *)malloc(sizeof(uint64_t) * M);
            size_t k = 0;
            for (uint64_t i = 2; i <= limit && k < M; i++) if (!comp[i]) pr[k++] = i;
            free(comp);
            break;
        }
        free(comp);
        limit *= 2;
    }

    unsigned long long *ex = (unsigned long long *)calloc(M, sizeof(unsigned long long));
    unsigned long long *nx = (unsigned long long *)calloc(M, sizeof(unsigned long long));
    /* primorial inflation: prime(k)^e -> exponents +e on indices 0..k-1 */
    size_t idx = 0;
    for (int f = 0; f < fc; f++) {
        while (pr[idx] != fp[f]) idx++;
        for (size_t i = 0; i <= idx; i++) ex[i] += fe[f];
    }

    size_t top = idx + 1; /* number of used indices */
    seq_t count = 0;
    for (;;) {
        long double ratio = 1.0L;
        for (size_t i = 0; i < top; i++) {
            if (ex[i]) {
                long double p = (long double)pr[i];
                long double t = (1.0L - powl(p, -(long double)(ex[i] + 1))) / (1.0L - 1.0L / p);
                ratio *= t;
            }
        }
        if (ratio < 2.0L - 1e-12L) break;
        if (top + 1 >= M) break;
        for (size_t i = 0; i < M; i++) nx[i] = 0;
        for (size_t i = 0; i < top; i++) nx[i + 1] = ex[i];
        unsigned long long *t = ex; ex = nx; nx = t;
        top++;
        count++;
    }
    free(ex); free(nx); free(pr);
    return count;
}
