/*
 * A302040  Numbers k such that A078898(k) is a power of 2; an analog for
 *          A000961 based on factorization-kind of process involving the sieve
 *          of Eratosthenes (A083221).
 * https://oeis.org/A302040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve search: a(n) = n-th k such that A078898(k) is a power of 2
 *            (A078898(k) = number of m <= k with lpf(m) = lpf(k); k = 1 is
 *            included)
 * Range:     u8 n <= 77, u16 n <= 6881, u32 n <= 6883, u64 n <= 6883,
 *            u128 n <= 6883, big any n
 * Validated: standard, pass, 95 terms through n = 6539 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *A302040_terms = NULL;
static size_t A302040_count = 0;

static void A302040_build(uint64_t limit)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    uint32_t *cnt = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    uint64_t *terms = (uint64_t *)malloc(((size_t)limit + 1) * sizeof(uint64_t));
    if (!spf || !cnt || !terms) abort();
    for (uint64_t i = 2; i <= limit; i++) {
        if (spf[i] == 0) {
            for (uint64_t j = i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    size_t found = 0;
    for (uint64_t k = 1; k <= limit; k++) {
        bool ok;
        if (k == 1) {
            ok = true;
        } else {
            uint32_t p = spf[k];
            uint32_t c = ++cnt[p];
            ok = (c & (c - 1)) == 0;
        }
        if (ok) terms[found++] = k;
    }
    free(spf);
    free(cnt);
    free(A302040_terms);
    A302040_terms = terms;
    A302040_count = found;
}

seq_t a(idx_t n)
{
    uint64_t limit = 1024;
    while (A302040_count < (size_t)n) {
        A302040_build(limit);
        limit *= 2;
    }
    return (seq_t)A302040_terms[(size_t)n - 1];
}
