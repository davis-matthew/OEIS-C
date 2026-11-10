/*
 * A353333  Number of ways to write n as a product of the terms of A340784
 *          larger than 1; a(1) = 1 by convention (an empty product).
 * https://oeis.org/A353333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of factorizations of n into factors k > 1 with
 *            A001222(k) and A056239(k) both even (a(1) = 1); computed by a
 *            coin-change DP over the divisors of n
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8652 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

typedef struct { uint64_t d; unsigned om; unsigned w; } A353333_div;

static int A353333_cmp(const void *x, const void *y) {
    uint64_t a = ((const A353333_div *)x)->d, b = ((const A353333_div *)y)->d;
    return (a < b) ? -1 : (a > b);
}

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    uint64_t pr[20]; unsigned ex[20]; unsigned par[20];
    int cnt = 0;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            pr[cnt] = p; ex[cnt] = e; cnt++;
        }
    }
    if (m > 1) { pr[cnt] = m; ex[cnt] = 1; cnt++; }
    /* prime indices (parity) via sieve up to the largest prime factor */
    uint64_t mx = 0;
    for (int i = 0; i < cnt; i++) if (pr[i] > mx) mx = pr[i];
    unsigned char *comp = (unsigned char *)calloc((size_t)mx + 1, 1);
    if (!comp) return 0;
    for (int i = 0; i < cnt; i++) par[i] = 0;
    {
        uint64_t index = 0;
        for (uint64_t i = 2; i <= mx; i++) {
            if (!comp[i]) {
                index++;
                for (int k = 0; k < cnt; k++)
                    if (pr[k] == i) par[k] = (unsigned)(index & 1);
                if (i <= mx / i)
                    for (uint64_t j = i * i; j <= mx; j += i) comp[j] = 1;
            }
        }
    }
    free(comp);
    size_t D = 1;
    for (int i = 0; i < cnt; i++) D *= (ex[i] + 1);
    A353333_div *dv = (A353333_div *)malloc(D * sizeof(A353333_div));
    if (!dv) return 0;
    size_t sz = 1;
    dv[0].d = 1; dv[0].om = 0; dv[0].w = 0;
    for (int i = 0; i < cnt; i++) {
        size_t base = sz;
        uint64_t pw = 1;
        for (unsigned e = 1; e <= ex[i]; e++) {
            pw *= pr[i];
            for (size_t j = 0; j < base; j++) {
                dv[sz].d = dv[j].d * pw;
                dv[sz].om = dv[j].om + e;
                dv[sz].w = dv[j].w + e * par[i];
                sz++;
            }
        }
    }
    qsort(dv, D, sizeof(A353333_div), A353333_cmp);
    seq_t *ways = mem_new(seq_t, D);
    ways[0] = 1;
    for (size_t g = 1; g < D; g++) {
        if ((dv[g].om & 1) || (dv[g].w & 1)) continue;
        uint64_t f = dv[g].d;
        for (size_t i = g; i < D; i++) {
            if (dv[i].d % f == 0) {
                uint64_t q = dv[i].d / f;
                size_t lo = 0, hi = i;
                while (lo < hi) {
                    size_t mid = lo + (hi - lo) / 2;
                    if (dv[mid].d < q) lo = mid + 1; else hi = mid;
                }
                ways[i] += ways[lo];
            }
        }
    }
    seq_t res = ways[D - 1];
    mem_free(ways);
    free(dv);
    return res;
}
