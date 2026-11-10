/*
 * A353359  Number of ways to write the cube of n as a product of the terms of
 *          A332820 larger than 1; a(1) = 1 by convention (an empty product).
 * https://oeis.org/A353359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of multiset factorizations of n^3 into factors k>1
 *            with A048675(k) == 0 (mod 3); computed by unbounded-knapsack DP
 *            over the divisor lattice of n^3 (A048675(k) mod 3 = alternating
 *            exponent sum)
 * Range:     u8 n <= 179, u16 n <= 5039, u32 n <= 16415, u64 n <= 16415,
 *            u128 n <= 16415, big any n
 * Validated: standard, pass, 115 terms through n = 9735 (2026-11-10)
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
    uint64_t pr[20];
    int ex[20];
    int sg[20];
    int k = 0;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            pr[k] = p; ex[k] = 3 * e; k++;
        }
    }
    if (m > 1) { pr[k] = m; ex[k] = 3; k++; }
    if (k == 0) return (seq_t)1;

    for (int j = 0; j < k; j++) {
        long long pi = (long long)prime_pi((idx_t)pr[j]);
        sg[j] = (pi & 1) ? 1 : 2;
    }

    size_t D = 1;
    for (int j = 0; j < k; j++) D *= (size_t)(ex[j] + 1);

    unsigned char *dec = (unsigned char *)malloc(D * (size_t)k);
    unsigned char *chi = (unsigned char *)malloc(D);
    if (!dec || !chi) { free(dec); free(chi); return (seq_t)0; }

    for (size_t i = 0; i < D; i++) {
        size_t t = i;
        int s = 0;
        for (int j = 0; j < k; j++) {
            int r = ex[j] + 1;
            int v = (int)(t % (size_t)r);
            t /= (size_t)r;
            dec[i * (size_t)k + j] = (unsigned char)v;
            s += v * sg[j];
        }
        chi[i] = (unsigned char)(s % 3);
    }

    seq_t *ways = mem_new(seq_t, D);
    ways[0] = 1;
    for (size_t d = 1; d < D; d++) {
        if (chi[d] != 0) continue;
        const unsigned char *dd = dec + d * (size_t)k;
        for (size_t x = d; x < D; x++) {
            const unsigned char *xx = dec + x * (size_t)k;
            bool ok = true;
            for (int j = 0; j < k; j++) {
                if (xx[j] < dd[j]) { ok = false; break; }
            }
            if (ok) ways[x] += ways[x - d];
        }
    }
    seq_t res = ways[D - 1];
    mem_free(ways);
    free(dec);
    free(chi);
    return res;
}
