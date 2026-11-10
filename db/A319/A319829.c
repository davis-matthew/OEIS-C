/*
 * A319829  FDH numbers of strict integer partitions of odd numbers.
 * https://oeis.org/A319829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 2 whose FDH partition (distinct Fermi-Dirac
 *            primes f(y_i) with product m) has odd sum y_1+...+y_k; computed
 *            by sieve
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *fdh_terms = NULL;
static uint64_t fdh_count = 0;

static void fdh_build(uint64_t L) {
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    uint32_t *idx = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    uint64_t i, j;
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= L; j += i) if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (i = 2; i <= L; i++) {
        if (spf[i] == i) {
            uint64_t v = i;
            while (v <= L) {
                idx[v] = 1;
                v = v * v;
            }
        }
    }
    {
        uint32_t c = 0;
        for (i = 2; i <= L; i++) if (idx[i]) idx[i] = ++c;
    }
    free(fdh_terms);
    fdh_terms = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)L + 1));
    fdh_count = 0;
    for (i = 2; i <= L; i++) {
        uint64_t m = i;
        uint64_t par = 0;
        while (m > 1) {
            uint64_t p = spf[m];
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            uint64_t v = p;
            while (e) {
                if (e & 1) par += idx[v];
                e >>= 1;
                if (e) v = v * v;
            }
        }
        if (par & 1) fdh_terms[fdh_count++] = (uint32_t)i;
    }
    free(spf);
    free(idx);
}

seq_t a(idx_t n) {
    static uint64_t L = 0;
    if (n == 0) return 0;
    while (fdh_count < (uint64_t)n) {
        L = L ? L * 2 : 1024;
        fdh_build(L);
    }
    return (seq_t)fdh_terms[n - 1];
}
