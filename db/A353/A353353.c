/*
 * A353353  Number of ways to write n as a product of the terms of A332820
 *          larger than 1; a(1) = 1 by convention (an empty product).
 * https://oeis.org/A353353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of factorizations of n into factors k > 1 with
 *            A048675(k) == 0 (mod 3); a(1) = 1. Computed by a knapsack DP
 *            over the divisor lattice of n
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 126 terms through n = 9611 (2026-11-10)
 * Helpers:   mem, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    /* factorize n by trial division */
    uint64_t pr[64];
    int ex[64];
    int wt[64];
    int k = 0;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            pr[k] = p; ex[k] = e; k++;
        }
    }
    if (m > 1) { pr[k] = m; ex[k] = 1; k++; }

    /* weight of prime p mod 3: 2^(pi-1) mod 3 -> 1 if pi odd, 2 if even */
    for (int j = 0; j < k; j++) {
        unsigned long long pi = (unsigned long long)prime_pi((idx_t)pr[j]);
        wt[j] = (pi & 1ULL) ? 1 : 2;
    }

    /* mixed-radix strides */
    size_t stride[64];
    size_t D = 1;
    for (int j = 0; j < k; j++) {
        stride[j] = D;
        D *= (size_t)(ex[j] + 1);
    }

    /* exponent vectors and weights of each divisor */
    int *vec = (int *)malloc(D * (size_t)k * sizeof(int));
    int *dw = (int *)malloc(D * sizeof(int));
    if (!vec || !dw) { free(vec); free(dw); return (seq_t)0; }
    for (size_t d = 0; d < D; d++) {
        size_t r = d;
        int w = 0;
        for (int j = 0; j < k; j++) {
            int base = ex[j] + 1;
            int e = (int)(r % (size_t)base);
            r /= (size_t)base;
            vec[d * (size_t)k + j] = e;
            w += e * wt[j];
        }
        dw[d] = w % 3;
    }

    seq_t *dp = mem_new(seq_t, D);
    dp[0] = (seq_t)1;

    for (size_t g = 1; g < D; g++) {
        if (dw[g] != 0) continue;
        const int *vg = &vec[g * (size_t)k];
        for (size_t d = g; d < D; d++) {
            const int *vd = &vec[d * (size_t)k];
            bool ok = true;
            for (int j = 0; j < k; j++) {
                if (vd[j] < vg[j]) { ok = false; break; }
            }
            if (ok) dp[d] += dp[d - g];
        }
    }

    seq_t result = dp[D - 1];
    mem_free(dp);
    free(vec);
    free(dw);
    return result;
}
