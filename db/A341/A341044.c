/*
 * A341044  Numbers k such that A318996(k) is prime.
 * https://oeis.org/A341044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A318996(k) = Sum_{d|k} (sigma(k) mod d)
 *            prime; found by sieve over k, doubling the limit as needed
 * Range:     u8 n <= 68, u16 n <= 7840, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 8675 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool A341044_isp(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

static uint64_t *A341044_terms = NULL;
static size_t A341044_cnt = 0;
static uint64_t A341044_lim = 0;

static void A341044_build(uint64_t L)
{
    uint64_t *sg = (uint64_t *)calloc(L + 1, sizeof(uint64_t));
    uint64_t *sm = (uint64_t *)calloc(L + 1, sizeof(uint64_t));
    if (!sg || !sm) abort();
    for (uint64_t d = 1; d <= L; ++d)
        for (uint64_t m = d; m <= L; m += d)
            sg[m] += d;
    for (uint64_t d = 1; d <= L; ++d)
        for (uint64_t m = d; m <= L; m += d)
            sm[m] += sg[m] % d;
    free(A341044_terms);
    A341044_terms = (uint64_t *)malloc((L + 1) * sizeof(uint64_t));
    if (!A341044_terms) abort();
    A341044_cnt = 0;
    for (uint64_t k = 1; k <= L; ++k)
        if (A341044_isp(sm[k]))
            A341044_terms[A341044_cnt++] = k;
    A341044_lim = L;
    free(sg);
    free(sm);
}

seq_t a(idx_t n)
{
    if (A341044_lim == 0) A341044_build(4096);
    while (A341044_cnt < (size_t)n)
        A341044_build(A341044_lim * 2);
    return (seq_t)A341044_terms[(size_t)n - 1];
}
