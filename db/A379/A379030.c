/*
 * A379030  Nonsquarefree modified exponential abundant numbers: nonsquarefree
 *          numbers k such that A241405(k) > 2*k.
 * https://oeis.org/A379030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k that is nonsquarefree with A241405(k) > 2*k;
 *            computed by sieve (smallest prime factor) with doubling limit
 * Range:     u8 n <= 3, u16 n <= 1361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9717 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A379030_terms = NULL;
static uint64_t A379030_count = 0;
static uint64_t A379030_limit = 0;

static void A379030_build(uint64_t limit)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    uint64_t *terms = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)limit / 20 + 16));
    uint64_t cap = limit / 20 + 16, cnt = 0;
    if (!spf || !terms) { free(spf); free(terms); return; }
    for (uint64_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            for (uint64_t j = i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (uint64_t k = 4; k <= limit; ++k) {
        uint64_t m = k, prod = 1;
        bool sqfree = true;
        while (m > 1) {
            uint64_t p = spf[m];
            unsigned e = 0;
            while (m % p == 0) { m /= p; ++e; }
            if (e > 1) sqfree = false;
            uint64_t s = 0;
            unsigned r = e + 1;
            for (unsigned d = 1; d <= r; ++d) {
                if (r % d == 0) {
                    uint64_t pw = 1;
                    for (unsigned t = 0; t + 1 < d; ++t) pw *= p;
                    s += pw;
                }
            }
            prod *= s;
        }
        if (!sqfree && prod > 2 * k) {
            if (cnt >= cap) {
                cap *= 2;
                uint64_t *t2 = (uint64_t *)realloc(terms, sizeof(uint64_t) * (size_t)cap);
                if (!t2) { free(spf); free(terms); return; }
                terms = t2;
            }
            terms[cnt++] = k;
        }
    }
    free(spf);
    free(A379030_terms);
    A379030_terms = terms;
    A379030_count = cnt;
    A379030_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A379030_limit == 0) A379030_build(4096);
    while (A379030_count < (uint64_t)n) {
        uint64_t old = A379030_limit;
        A379030_build(old * 2);
        if (A379030_limit == old) return 0;
    }
    return (seq_t)A379030_terms[n - 1];
}
