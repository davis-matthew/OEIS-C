/*
 * A383469  Semiprimes k such that A001358(k) + k and A001358(k) - k are also
 *          semiprimes.
 * https://oeis.org/A383469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th semiprime k such that A001358(k)-k and
 *            A001358(k)+k are also semiprimes; found with a growing sieve of
 *            bigomega values
 * Range:     u8 n <= 8, u16 n <= 516, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9452 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *A383469_terms = NULL;
static size_t A383469_cnt = 0;
static uint64_t A383469_M = 0;

/* Sieve the number of prime factors with multiplicity, Omega(m), for 0..M (each value stored in a uint8_t) */
static unsigned char *A383469_sieve(uint64_t M) {
    unsigned char *om = (unsigned char *)calloc((size_t)(M + 1), 1);
    if (!om) return NULL;
    for (uint64_t p = 2; p <= M; p++) {
        if (om[p] != 0) continue; /* composite: already marked by a smaller prime */
        for (uint64_t q = p; q <= M; ) {
            for (uint64_t m = q; m <= M; m += q) om[m]++;
            if (q > M / p) break;
            q *= p;
        }
    }
    return om;
}

static void A383469_build(uint64_t M) {
    unsigned char *om = A383469_sieve(M);
    if (!om) return;
    size_t cap = 1024, cnt = 0;
    uint64_t *t = (uint64_t *)malloc(cap * sizeof(uint64_t));
    uint64_t s = 3; /* last semiprime examined */
    for (uint64_t k = 1; ; k++) {
        do { s++; } while (s <= M && om[s] != 2);
        if (s > M || s + k > M) break;
        /* s = A001358(k) */
        if (s <= k) continue;
        if (om[k] != 2) continue;
        if (om[s - k] != 2) continue;
        if (om[s + k] != 2) continue;
        if (cnt == cap) {
            cap *= 2;
            t = (uint64_t *)realloc(t, cap * sizeof(uint64_t));
        }
        t[cnt++] = k;
    }
    free(om);
    free(A383469_terms);
    A383469_terms = t;
    A383469_cnt = cnt;
    A383469_M = M;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    while ((uint64_t)A383469_cnt < (uint64_t)n) {
        uint64_t M = A383469_M ? A383469_M * 2 : ((uint64_t)1 << 16);
        A383469_build(M);
    }
    return (seq_t)A383469_terms[n - 1];
}
