/*
 * A353332  Number of divisors d of n for which both A001222(d) and A056239(d)
 *          are even.
 * https://oeis.org/A353332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [A001222(d) even and A056239(d) even]; computed
 *            by a parity DP over the prime-power factorization of n
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9815 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    uint64_t ps[24];
    int es[24];
    int k = 0;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            ps[k] = p; es[k] = e; k++;
        }
    }
    if (m > 1) { ps[k] = m; es[k] = 1; k++; }

    int par[24];
    for (int i = 0; i < k; i++) par[i] = 0;
    if (k > 0) {
        uint64_t maxp = ps[0];
        for (int i = 1; i < k; i++) if (ps[i] > maxp) maxp = ps[i];
        uint8_t *sv = (uint8_t *)malloc((size_t)maxp + 1);
        if (!sv) return (seq_t)0;
        memset(sv, 1, (size_t)maxp + 1);
        uint64_t idx = 0;
        for (uint64_t i = 2; i <= maxp; i++) {
            if (sv[i]) {
                idx++;
                if (i <= maxp / i) {
                    for (uint64_t j = i * i; j <= maxp; j += i) sv[j] = 0;
                }
                for (int t = 0; t < k; t++)
                    if (ps[t] == i) par[t] = (int)(idx & 1);
            }
        }
        free(sv);
    }

    seq_t cnt[4];
    for (int s = 0; s < 4; s++) cnt[s] = (seq_t)0;
    cnt[0] = (seq_t)1;
    for (int i = 0; i < k; i++) {
        seq_t nw[4];
        for (int s = 0; s < 4; s++) nw[s] = (seq_t)0;
        for (int j = 0; j <= es[i]; j++) {
            int o = j & 1;
            int x = o | ((o & par[i]) << 1);
            for (int s = 0; s < 4; s++) nw[s ^ x] += cnt[s];
        }
        for (int s = 0; s < 4; s++) cnt[s] = nw[s];
    }
    return cnt[0];
}
