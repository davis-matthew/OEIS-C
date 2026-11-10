/*
 * A347877  Numbers k for which A003415(sigma(k)) is odd.
 * https://oeis.org/A347877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A003415(sigma(k)) is odd; found by a
 *            sieve of sigma with doubling limit. m' is odd iff m == 2 (mod
 *            4), or m odd with bigomega(m) odd
 * Range:     u8 n <= 74, u16 n <= 9651, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 86 terms through n = 9528 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static uint64_t *A347877_terms = NULL;
static uint64_t A347877_cnt = 0;
static uint64_t A347877_limit = 0;

static bool A347877_der_odd(uint64_t m) {
    if (m % 4 == 2) return true;
    if (m % 2 == 0) return false;
    int c = 0;
    for (uint64_t p = 3; p * p <= m; p += 2) {
        while (m % p == 0) { m /= p; c++; }
    }
    if (m > 1) c++;
    return (c & 1) != 0;
}

static void A347877_build(uint64_t L) {
    uint64_t *sg = (uint64_t *)malloc((L + 1) * sizeof(uint64_t));
    memset(sg, 0, (L + 1) * sizeof(uint64_t));
    for (uint64_t d = 1; d <= L; d++)
        for (uint64_t j = d; j <= L; j += d) sg[j] += d;
    free(A347877_terms);
    A347877_terms = (uint64_t *)malloc((L + 1) * sizeof(uint64_t));
    A347877_cnt = 0;
    for (uint64_t k = 1; k <= L; k++) {
        if (A347877_der_odd(sg[k])) A347877_terms[A347877_cnt++] = k;
    }
    free(sg);
    A347877_limit = L;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (A347877_limit == 0) A347877_build(4096);
    while ((uint64_t)n > A347877_cnt) A347877_build(A347877_limit * 2);
    return (seq_t)A347877_terms[n - 1];
}
