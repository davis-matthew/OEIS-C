/*
 * A319828  FDH numbers of strict integer partitions of even numbers.
 * https://oeis.org/A319828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m (m>=1, ascending) whose FDH-partition has even
 *            sum, i.e. the sum of the indices of the Fermi-Dirac primes
 *            (A050376) in m's binary-exponent factorization is even; found by
 *            a sieve with doubling limit
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *fdh_terms = NULL;
static size_t fdh_cnt = 0;
static uint64_t fdh_lim = 0;

static void fdh_build(uint64_t L) {
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    unsigned char *isfd = (unsigned char *)calloc((size_t)L + 1, 1);
    unsigned char *par = (unsigned char *)calloc((size_t)L + 1, 1);
    unsigned char *xr = (unsigned char *)calloc((size_t)L + 1, 1);
    uint64_t m, c;
    for (m = 2; m <= L; m++) {
        if (spf[m] == 0) {
            uint64_t j;
            for (j = m; j <= L; j += m) if (spf[j] == 0) spf[j] = (uint32_t)m;
        }
    }
    for (m = 2; m <= L; m++) {
        if (spf[m] == m) {
            uint64_t q = m;
            isfd[q] = 1;
            while (q <= L / q) { q *= q; isfd[q] = 1; }
        }
    }
    c = 0;
    for (m = 1; m <= L; m++) {
        if (isfd[m]) c++;
        par[m] = (unsigned char)(c & 1);
    }
    xr[1] = 0;
    for (m = 2; m <= L; m++) {
        uint64_t p = spf[m], r = m, q = p;
        unsigned e = 0;
        unsigned char acc;
        while (r % p == 0) { r /= p; e++; }
        acc = xr[r];
        while (e) {
            if (e & 1) acc ^= par[q];
            e >>= 1;
            if (e) q *= q;
        }
        xr[m] = acc;
    }
    free(fdh_terms);
    fdh_terms = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)L + 1));
    fdh_cnt = 0;
    for (m = 1; m <= L; m++) if (!xr[m]) fdh_terms[fdh_cnt++] = m;
    fdh_lim = L;
    free(spf); free(isfd); free(par); free(xr);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (fdh_lim == 0) fdh_build(256);
    while (fdh_cnt < (size_t)n) fdh_build(fdh_lim * 2);
    return (seq_t)fdh_terms[(size_t)n - 1];
}
