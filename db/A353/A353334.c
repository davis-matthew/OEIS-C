/*
 * A353334  Number of factorizations of the square of n into factors k > 1 for
 *          which both A001222(k) and A056239(k) are even.
 * https://oeis.org/A353334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of multisets of divisors k>1 of n^2 with Omega(k)
 *            and A056239(k) both even whose product is n^2 (knapsack DP over
 *            the divisors of n^2)
 * Range:     u8 n <= 1319, u16 n <= 10080, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 118 terms through n = 7748 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static int A353334_prime_index_odd(uint64_t p) {
    /* returns 1 if p is the prime with odd index (2 is index 1) */
    char *s = (char *)calloc((size_t)p + 1, 1);
    if (!s) return 0;
    uint64_t cnt = 0;
    for (uint64_t i = 2; i <= p; i++) {
        if (!s[i]) {
            cnt++;
            for (uint64_t j = i * i; j <= p && j >= i; j += i) s[j] = 1;
        }
    }
    free(s);
    return (int)(cnt & 1);
}

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    uint64_t pr[24];
    int ex[24];
    int par[24];
    int k = 0;
    uint64_t t = n;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; e++; }
            pr[k] = p; ex[k] = 2 * e; k++;
        }
    }
    if (t > 1) { pr[k] = t; ex[k] = 2; k++; }
    for (int i = 0; i < k; i++) par[i] = A353334_prime_index_odd(pr[i]);

    size_t D = 1;
    for (int i = 0; i < k; i++) D *= (size_t)(ex[i] + 1);

    int *dg = (int *)malloc(D * (size_t)k * sizeof(int));
    if (!dg) return 0;
    for (size_t d = 0; d < D; d++) {
        size_t r = d;
        for (int i = 0; i < k; i++) {
            dg[d * k + i] = (int)(r % (size_t)(ex[i] + 1));
            r /= (size_t)(ex[i] + 1);
        }
    }
    seq_t *ways = mem_new(seq_t, D);
    ways[0] = 1;
    for (size_t g = 1; g < D; g++) {
        int s0 = 0, s1 = 0;
        for (int i = 0; i < k; i++) {
            if (par[i]) s0 += dg[g * k + i]; else s1 += dg[g * k + i];
        }
        if ((s0 & 1) || (s1 & 1)) continue;
        for (size_t d = g; d < D; d++) {
            int ok = 1;
            for (int i = 0; i < k; i++) {
                if (dg[d * k + i] < dg[g * k + i]) { ok = 0; break; }
            }
            if (ok) ways[d] += ways[d - g];
        }
    }
    seq_t res = ways[D - 1];
    mem_free(ways);
    free(dg);
    return res;
}
