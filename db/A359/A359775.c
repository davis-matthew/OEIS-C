/*
 * A359775  Positions of odd terms in A359773, where A359773 is the Dirichlet
 *          inverse of A356163.
 * https://oeis.org/A359775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = position of the n-th odd term of A359773, the Dirichlet
 *            inverse of A356163. Computed by a sieve of the Dirichlet inverse
 *            mod 2, with the limit doubled until n terms are found
 * Range:     u8 n <= 79, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 8845 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A359775_pos = 0;
static uint64_t A359775_cnt = 0;
static uint64_t A359775_lim = 0;

static void A359775_build(uint64_t L)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    unsigned char *par = (unsigned char *)calloc((size_t)L + 1, 1);
    unsigned char *f = (unsigned char *)calloc((size_t)L + 1, 1);
    unsigned char *acc = (unsigned char *)calloc((size_t)L + 1, 1);
    uint64_t i, j, m, d;

    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    f[1] = 1;
    for (i = 2; i <= L; i++) {
        par[i] = par[i / spf[i]] ^ (unsigned char)(spf[i] & 1);
        f[i] = par[i] ? 0 : 1;
    }

    free(A359775_pos);
    A359775_pos = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)L + 1));
    A359775_cnt = 0;

    for (m = 1; m <= L; m++) {
        unsigned char g = (m == 1) ? 1 : acc[m];
        if (g) {
            A359775_pos[A359775_cnt++] = m;
            for (d = 2; d * m <= L; d++)
                if (f[d]) acc[d * m] ^= 1;
        }
    }
    A359775_lim = L;
    free(spf); free(par); free(f); free(acc);
}

seq_t a(idx_t n)
{
    uint64_t L = A359775_lim ? A359775_lim : 65536;
    if (A359775_lim == 0 || A359775_cnt < (uint64_t)n) {
        while (1) {
            A359775_build(L);
            if (A359775_cnt >= (uint64_t)n) break;
            L *= 2;
        }
    }
    return (seq_t)A359775_pos[n - 1];
}
