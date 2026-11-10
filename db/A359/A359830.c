/*
 * A359830  Numbers k such that A048675(k) is not a multiple of 3.
 * https://oeis.org/A359830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve search: k is a term iff A048675(k) mod 3 != 0, where
 *            A048675(k) mod 3 = Sum_{p^e || k} e * 2^(PrimePi(p)-1) mod 3,
 *            and 2^(i-1) mod 3 = 1 for odd i, 2 for even i
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 77 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *A359830_terms = NULL;
static size_t A359830_nterms = 0;
static uint64_t A359830_limit = 0;

static void A359830_build(uint64_t L)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    unsigned char *w = (unsigned char *)calloc((size_t)(L + 1), 1);   /* weight of prime p */
    unsigned char *v = (unsigned char *)calloc((size_t)(L + 1), 1);   /* A048675(k) mod 3 */
    uint32_t *primes = (uint32_t *)malloc((size_t)(L + 1) * sizeof(uint32_t));
    size_t np = 0;
    uint64_t i;
    size_t j;

    if (A359830_terms) free(A359830_terms);
    A359830_terms = (uint64_t *)malloc((size_t)(L + 1) * sizeof(uint64_t));
    A359830_nterms = 0;

    v[1] = 0;
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            primes[np++] = (uint32_t)i;
            w[i] = (np & 1) ? 1 : 2;   /* np = PrimePi(i) */
        }
        for (j = 0; j < np; j++) {
            uint64_t p = primes[j];
            if (p > spf[i] || p * i > L) break;
            spf[p * i] = (uint32_t)p;
        }
        {
            uint64_t p = spf[i];
            v[i] = (unsigned char)((v[i / p] + w[p]) % 3);
        }
        if (v[i] != 0) A359830_terms[A359830_nterms++] = i;
    }

    free(spf);
    free(w);
    free(v);
    free(primes);
    A359830_limit = L;
}

seq_t a(idx_t n)
{
    uint64_t L = A359830_limit ? A359830_limit : 1024;
    while (A359830_nterms < (size_t)n) {
        if (A359830_limit != 0) L *= 2;
        A359830_build(L);
    }
    return (seq_t)A359830_terms[n - 1];
}
