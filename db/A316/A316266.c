/*
 * A316266  FDH numbers of strict integer partitions with prime parts and
 *          prime length.
 * https://oeis.org/A316266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number whose FDH strict partition has all parts
 *            prime and a prime number of parts (search/sieve over m with
 *            Fermi-Dirac factorization)
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *fdh_terms = NULL;
static size_t fdh_cnt = 0;
static uint64_t fdh_limit = 0;

static bool fdh_small_prime(uint64_t x) {
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

static void fdh_build(uint64_t L) {
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    uint32_t *rk = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    unsigned char *fd = (unsigned char *)calloc((size_t)L + 1, 1);
    uint64_t i, j;
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
            uint64_t q = i;
            while (q <= L) {
                fd[q] = 1;
                if (q > 0xFFFFFFFFULL) break;
                q = q * q;
            }
        }
    }
    uint32_t c = 0;
    for (i = 1; i <= L; i++) {
        if (fd[i]) c++;
        rk[i] = c;
    }
    free(fdh_terms);
    fdh_terms = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)L + 1));
    fdh_cnt = 0;
    for (uint64_t m = 2; m <= L; m++) {
        uint64_t x = m;
        bool ok = true;
        unsigned parts = 0;
        while (x > 1 && ok) {
            uint64_t p = spf[x];
            unsigned e = 0;
            while (x % p == 0) { x /= p; e++; }
            uint64_t q = p;
            while (e) {
                if (e & 1) {
                    parts++;
                    if (!fdh_small_prime(rk[q])) { ok = false; break; }
                }
                e >>= 1;
                if (e) q = q * q;
            }
        }
        if (ok && fdh_small_prime(parts)) fdh_terms[fdh_cnt++] = m;
    }
    fdh_limit = L;
    free(spf); free(rk); free(fd);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (fdh_limit == 0) fdh_build(4096);
    while (fdh_cnt < (size_t)n) fdh_build(fdh_limit * 2);
    return (seq_t)fdh_terms[(size_t)n - 1];
}
