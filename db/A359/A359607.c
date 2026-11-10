/*
 * A359607  Terms of A046337 for which A358777 is zero, where the latter is
 *          the Dirichlet inverse of former's characteristic function.
 * https://oeis.org/A359607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve; A046337 terms m (odd, Omega(m) even) with A358777(m)=0,
 *            where A358777 = Dirichlet inverse of the characteristic function
 *            f of A046337 (b(1)=1, b(m) = -Sum_{d|m, d<m} b(d)*f(m/d))
 * Range:     u8 n <= 1, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t *A359607_terms = NULL;
static size_t A359607_count = 0;
static uint64_t A359607_limit = 0;

static void A359607_build(uint64_t L)
{
    uint32_t *spf = (uint32_t *)malloc((size_t)(L + 1) * sizeof(uint32_t));
    unsigned char *f = (unsigned char *)malloc((size_t)(L + 1));
    uint64_t *acc = (uint64_t *)malloc((size_t)(L + 1) * sizeof(uint64_t));
    uint64_t *out = (uint64_t *)malloc(sizeof(uint64_t) * 16);
    size_t cap = 16, cnt = 0;
    uint64_t i, j, m, k;
    if (!spf || !f || !acc || !out) exit(EXIT_FAILURE);
    for (i = 0; i <= L; i++) { spf[i] = 0; acc[i] = 0; f[i] = 0; }
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i <= L / i)
                for (j = i * i; j <= L; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    /* f[i] holds parity of Omega, then converted */
    f[1] = 1; /* temporarily: parity stored as (Omega even) flag */
    for (i = 2; i <= L; i++) {
        uint64_t q = i / spf[i];
        f[i] = (unsigned char)(f[q] ^ 1);
    }
    /* f[i]==1 means Omega even; restrict to odd */
    for (i = 2; i <= L; i += 2) f[i] = 0;

    for (m = 1; m <= L; m += 2) {
        uint64_t b = (m == 1) ? 1 : (uint64_t)0 - acc[m];
        if (b == 0) {
            if (f[m]) {
                if (cnt == cap) {
                    cap *= 2;
                    out = (uint64_t *)realloc(out, cap * sizeof(uint64_t));
                    if (!out) exit(EXIT_FAILURE);
                }
                out[cnt++] = m;
            }
        } else {
            for (k = 3; k <= L / m; k += 2)
                if (f[k]) acc[m * k] += b;
        }
    }
    free(spf); free(f); free(acc);
    free(A359607_terms);
    A359607_terms = out;
    A359607_count = cnt;
    A359607_limit = L;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A359607_limit == 0) A359607_build(1ULL << 16);
    while (A359607_count < (size_t)n)
        A359607_build(A359607_limit * 2);
    return (seq_t)A359607_terms[n - 1];
}
