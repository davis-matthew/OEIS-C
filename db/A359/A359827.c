/*
 * A359827  Positions of odd terms in A353348, where A353348 is Dirichlet
 *          inverse of the characteristic function for numbers k where
 *          A048675(k) is a multiple of 3.
 * https://oeis.org/A359827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th odd term of A353348, where A353348
 *            is the Dirichlet inverse of f(k) = [A048675(k) == 0 mod 3];
 *            computed by a sieve of b(m) mod 2 with b(1)=1, b(m) = -Sum_{d|m,
 *            d<m} b(d)*f(m/d), doubling the limit as needed
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *A359827_pos = 0;
static uint64_t A359827_cnt = 0;
static uint64_t A359827_lim = 0;

static void A359827_build(uint64_t L)
{
    unsigned char *s = (unsigned char *)calloc(L + 1, 1);
    unsigned char *comp = (unsigned char *)calloc(L + 1, 1);
    unsigned char *b = (unsigned char *)calloc(L + 1, 1);
    uint32_t *pr = (uint32_t *)malloc(sizeof(uint32_t) * (L + 1));
    size_t np = 0;
    uint64_t k, d, m;

    for (k = 2; k <= L; k++) {
        if (!comp[k]) {
            pr[np++] = (uint32_t)k;
            s[k] = (np & 1) ? 1 : 2;
        }
        for (size_t j = 0; j < np; j++) {
            uint64_t p = pr[j];
            if (p * k > L) break;
            comp[p * k] = 1;
            s[p * k] = (unsigned char)((s[k] + ((j + 1) & 1 ? 1 : 2)) % 3);
            if (k % p == 0) break;
        }
    }
    /* s[k] = A048675(k) mod 3; f(k) = (s[k]==0) */
    b[1] = 1;
    for (d = 1; d <= L; d++) {
        if (!b[d]) continue;
        for (m = 2 * d; m <= L; m += d) {
            if (s[m / d] == 0) b[m] ^= 1;
        }
    }
    free(A359827_pos);
    A359827_pos = (uint64_t *)malloc(sizeof(uint64_t) * (L + 1));
    A359827_cnt = 0;
    for (k = 1; k <= L; k++)
        if (b[k]) A359827_pos[A359827_cnt++] = k;
    A359827_lim = L;
    free(s); free(comp); free(b); free(pr);
}

seq_t a(idx_t n)
{
    uint64_t L = A359827_lim ? A359827_lim : 1024;
    while (A359827_lim == 0 || A359827_cnt < (uint64_t)n) {
        if (A359827_lim != 0) L = A359827_lim * 2;
        A359827_build(L);
    }
    return (seq_t)A359827_pos[n - 1];
}
