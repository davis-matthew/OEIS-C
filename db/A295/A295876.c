/*
 * A295876  Restricted growth sequence transform of A023900, Product_{p|n}
 *          (1-p).
 * https://oeis.org/A295876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of A023900(n) = Product_{p|n}
 *            (1-p); values are labeled 1,2,3,... in order of first appearance
 *            (sieve over 1..n)
 * Range:     u8 n <= 572, u16 n <= 65536, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 114 terms through n = 8169 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *A295876_ids = 0;
static idx_t A295876_cap = 0;

static void A295876_build(idx_t N)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    uint32_t *mag = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    unsigned char *par = (unsigned char *)calloc((size_t)N + 1, 1);
    uint32_t *seen = (uint32_t *)calloc(2 * ((size_t)N + 1), sizeof(uint32_t));
    uint32_t *ids = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    idx_t i, j;
    uint32_t cnt = 0;

    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    mag[1] = 1;
    par[1] = 0;
    ids[1] = ++cnt;
    seen[0 * ((size_t)N + 1) + 1] = cnt;
    for (i = 2; i <= N; i++) {
        uint32_t p = spf[i];
        idx_t r = i / p;
        size_t k;
        while (r % p == 0) r /= p;
        mag[i] = mag[r] * (p - 1);
        par[i] = par[r] ^ 1;
        k = (size_t)par[i] * ((size_t)N + 1) + mag[i];
        if (seen[k] == 0) seen[k] = ++cnt;
        ids[i] = seen[k];
    }
    free(spf);
    free(mag);
    free(par);
    free(seen);
    if (A295876_ids) free(A295876_ids);
    A295876_ids = ids;
    A295876_cap = N;
}

seq_t a(idx_t n)
{
    if (n > A295876_cap || A295876_ids == 0) {
        idx_t N = A295876_cap * 2;
        if (N < n) N = n;
        if (N < 1024) N = 1024;
        A295876_build(N);
    }
    return (seq_t)A295876_ids[n];
}
