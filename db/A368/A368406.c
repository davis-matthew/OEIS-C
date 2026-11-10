/*
 * A368406  Numbers k such that A368405(k) = 0.
 * https://oeis.org/A368406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k with Sum_{j=1..k} mu_inf(j) = 0, where mu_inf(j) =
 *            (-1)^(sum of popcount of exponents in j); computed by a sieve
 *            with a growing limit
 * Range:     u8 n <= 12, u16 n <= 910, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9850 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    uint64_t limit = 4096;
    for (;;) {
        uint32_t *spf = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
        signed char *mu = (signed char*)malloc(limit + 1);
        if (!spf || !mu) { free(spf); free(mu); return 0; }
        for (uint64_t i = 0; i <= limit; ++i) spf[i] = 0;
        for (uint64_t i = 2; i <= limit; ++i) {
            if (!spf[i]) {
                for (uint64_t j = i; j <= limit; j += i)
                    if (!spf[j]) spf[j] = (uint32_t)i;
            }
        }
        mu[0] = 0;
        mu[1] = 1;
        long long sum = 1;
        idx_t cnt = 0;
        for (uint64_t i = 2; i <= limit; ++i) {
            uint64_t p = spf[i];
            uint64_t x = i;
            int e = 0;
            while (x % p == 0) { ++e; x /= p; }
            int pc = 0;
            for (int t = e; t; t >>= 1) pc += t & 1;
            mu[i] = (signed char)((pc & 1) ? -mu[x] : mu[x]);
            sum += mu[i];
            if (sum == 0) {
                ++cnt;
                if (cnt == n) {
                    free(spf); free(mu);
                    return (seq_t)i;
                }
            }
        }
        free(spf); free(mu);
        limit *= 4;
    }
}
