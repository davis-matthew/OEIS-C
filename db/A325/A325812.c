/*
 * A325812  Numbers k such that gcd(A034448(k)-k, k-A048146(k)) is equal to
 *          abs(k-A048146(k)).
 * https://oeis.org/A325812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with gcd(A034448(k)-k, k-A048146(k)) =
 *            |k-A048146(k)|; found by a sieve search (smallest-prime-factor
 *            sieve, limit doubled until n terms are found)
 * Range:     u8 n <= 10, u16 n <= 648, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 78 terms through n = 9449 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static uint64_t *A325812_terms = NULL;
static uint64_t A325812_count = 0;
static uint64_t A325812_limit = 0;

static void A325812_build(uint64_t limit)
{
    uint32_t *spf = (uint32_t *)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    uint64_t *terms = (uint64_t *)malloc((size_t)(limit + 1) * sizeof(uint64_t));
    uint64_t cnt = 0, i, j;
    for (i = 0; i <= limit; ++i) spf[i] = 0;
    for (i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            for (j = i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (uint64_t k = 1; k <= limit; ++k) {
        unsigned __int128 sg = 1, us = 1;
        uint64_t m = k;
        while (m > 1) {
            uint64_t p = spf[m];
            unsigned __int128 pe = 1, s = 1;
            while (m % p == 0) {
                m /= p;
                pe *= p;
                s += pe;
            }
            sg *= s;
            us *= (pe + 1);
        }
        __int128 A = (__int128)us - (__int128)k;
        __int128 B = (__int128)k - ((__int128)sg - (__int128)us);
        int ok;
        if (B == 0) ok = (A == 0);
        else ok = (A % B == 0);
        if (ok) terms[cnt++] = k;
    }
    free(spf);
    free(A325812_terms);
    A325812_terms = terms;
    A325812_count = cnt;
    A325812_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A325812_limit == 0) A325812_build(1u << 16);
    while ((uint64_t)n > A325812_count)
        A325812_build(A325812_limit * 2);
    return (seq_t)A325812_terms[(uint64_t)n - 1];
}
