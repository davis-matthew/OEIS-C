/*
 * A332446  Numbers k for which A087808(sigma(k)) is equal to A087808(2*k).
 * https://oeis.org/A332446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A087808(sigma(k)) = A087808(2*k); found
 *            by a divisor-sum sieve with a doubling limit
 * Range:     u8 n <= 18, u16 n <= 1686, u32 n <= 20751, u64 n <= 20751,
 *            u128 n <= 20751, big any n
 * Validated: standard, pass, 81 terms through n = 9850 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A332446_f(uint64_t n)
{
    /* A087808: process bits from the MSB: 0 -> 2x, 1 -> x+1 */
    uint64_t x = 0;
    int started = 0;
    for (int b = 63; b >= 0; b--) {
        uint64_t bit = (n >> b) & 1ULL;
        if (!started) {
            if (!bit) continue;
            started = 1;
        }
        if (bit) x = x + 1;
        else x = 2 * x;
    }
    return x;
}

static uint64_t *A332446_terms = 0;
static size_t A332446_cnt = 0;
static uint64_t A332446_limit = 0;

static void A332446_build(uint64_t L)
{
    uint64_t *sg = (uint64_t *)calloc((size_t)L + 1, sizeof(uint64_t));
    uint64_t *old = A332446_terms;
    size_t cap = 1024, c = 0;
    uint64_t *t = (uint64_t *)malloc(cap * sizeof(uint64_t));
    uint64_t i, j;
    for (i = 1; i <= L; i++)
        for (j = i; j <= L; j += i)
            sg[j] += i;
    for (i = 1; i <= L; i++) {
        if (A332446_f(sg[i]) == A332446_f(2 * i)) {
            if (c == cap) {
                cap *= 2;
                t = (uint64_t *)realloc(t, cap * sizeof(uint64_t));
            }
            t[c++] = i;
        }
    }
    free(sg);
    free(old);
    A332446_terms = t;
    A332446_cnt = c;
    A332446_limit = L;
}

seq_t a(idx_t n)
{
    while ((uint64_t)n > A332446_cnt) {
        uint64_t L = A332446_limit ? A332446_limit * 2 : 4096;
        A332446_build(L);
    }
    return (seq_t)A332446_terms[n - 1];
}
