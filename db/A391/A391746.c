/*
 * A391746  Number of terms of A391909 <= n.
 * https://oeis.org/A391746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A391747(k), where A391747(k) = 1 if |Sum_j
 *            (-1)^(j-1) e_j| <= 1 for k = Product p_j^e_j, else 0 (sieve +
 *            prefix sums)
 * Range:     u8 n <= 445, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 90 terms through n = 9604 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *pre_tab = NULL;
static uint64_t pre_cap = 0;

static void build_pre(uint64_t N)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    int32_t *d = (int32_t *)calloc((size_t)N + 1, sizeof(int32_t));
    uint32_t *pt = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    uint64_t i, j;
    uint64_t pidx = 0;
    for (i = 2; i * i <= N; i++) {
        if (spf[i] == 0) {
            for (j = i * i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            pidx++;
            d[i] = (pidx & 1) ? 1 : -1;
        } else {
            uint64_t p = spf[i];
            d[i] = d[i / p] + d[p];
        }
    }
    pt[0] = 0;
    if (N >= 1) pt[1] = 1;
    for (i = 2; i <= N; i++)
        pt[i] = pt[i - 1] + ((d[i] >= -1 && d[i] <= 1) ? 1u : 0u);
    free(spf);
    free(d);
    free(pre_tab);
    pre_tab = pt;
    pre_cap = N;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n > pre_cap) {
        uint64_t N = (uint64_t)n;
        if (N < 2 * pre_cap && 2 * pre_cap <= 50000000ULL) N = 2 * pre_cap;
        if (N < 1024) N = 1024;
        build_pre(N);
    }
    return (seq_t)pre_tab[n];
}
