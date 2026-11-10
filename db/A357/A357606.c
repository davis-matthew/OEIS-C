/*
 * A357606  Primitive terms of A357605: numbers in A357605 with no proper
 *          divisor in A357605.
 * https://oeis.org/A357606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A162296(k) > 2*k and no proper divisor d of k
 *            with A162296(d) > 2*d; computed by sieve with doubling limit
 * Range:     u8 n <= 8, u16 n <= 1196, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9685 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *A357606_terms = 0;
static size_t A357606_cnt = 0;
static size_t A357606_lim = 0;

static void A357606_build(size_t L)
{
    uint64_t *g = (uint64_t *)calloc(L + 1, sizeof(uint64_t));
    unsigned char *nsf = (unsigned char *)calloc(L + 1, 1);
    unsigned char *cov = (unsigned char *)calloc(L + 1, 1);
    size_t d, m;
    for (d = 2; d * d <= L; d++)
        for (m = d * d; m <= L; m += d * d)
            nsf[m] = 1;
    for (d = 4; d <= L; d++) {
        if (!nsf[d]) continue;
        for (m = d; m <= L; m += d)
            g[m] += d;
    }
    free(A357606_terms);
    A357606_terms = (uint64_t *)malloc(sizeof(uint64_t) * (L / 4 + 16));
    A357606_cnt = 0;
    for (d = 1; d <= L; d++) {
        if (cov[d]) continue;
        if (g[d] > 2 * (uint64_t)d) {
            A357606_terms[A357606_cnt++] = d;
            for (m = 2 * d; m <= L; m += d)
                cov[m] = 1;
        }
    }
    A357606_lim = L;
    free(g);
    free(nsf);
    free(cov);
}

seq_t a(idx_t n)
{
    size_t L = A357606_lim ? A357606_lim : 1024;
    while (A357606_lim == 0 || A357606_cnt < (size_t)n) {
        if (A357606_lim) L = A357606_lim * 2;
        A357606_build(L);
    }
    return (seq_t)A357606_terms[(size_t)n - 1];
}
