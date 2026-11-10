/*
 * A340376  Numbers k such that there are no 1-digits in the ternary expansion
 *          of A048673(k).
 * https://oeis.org/A340376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that A048673(k) = (A003961(k)+1)/2 has
 *            no digit 1 in base 3; computed by a linear sieve of A003961
 *            (completely multiplicative, p -> next prime) with a growing
 *            limit
 * Range:     u8 n <= 25, u16 n <= 842, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 82 terms through n = 9257 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

static uint32_t *A340376_terms = NULL;
static size_t A340376_cnt = 0;
static uint64_t A340376_limit = 0;

static void A340376_build(uint64_t limit)
{
    uint64_t *f = (uint64_t *)malloc((size_t)(limit + 1) * sizeof(uint64_t));
    uint32_t *pr = (uint32_t *)malloc((size_t)(limit / 4 + 100) * sizeof(uint32_t));
    uint32_t *res = (uint32_t *)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    size_t np = 0, rc = 0;
    uint64_t i;
    size_t j;
    for (i = 0; i <= limit; i++) f[i] = 0;
    f[1] = 1;
    for (i = 2; i <= limit; i++) {
        if (f[i] == 0) {
            f[i] = next_prime(i);
            pr[np++] = (uint32_t)i;
        }
        for (j = 0; j < np; j++) {
            uint64_t p = pr[j];
            if (p * i > limit) break;
            f[p * i] = f[i] * f[p];
            if (i % p == 0) break;
        }
    }
    for (i = 1; i <= limit; i++) {
        uint64_t v = (f[i] + 1) / 2;
        bool ok = true;
        while (v) {
            if (v % 3 == 1) { ok = false; break; }
            v /= 3;
        }
        if (ok) res[rc++] = (uint32_t)i;
    }
    free(f);
    free(pr);
    if (A340376_terms) free(A340376_terms);
    A340376_terms = res;
    A340376_cnt = rc;
    A340376_limit = limit;
}

seq_t a(idx_t n)
{
    if (A340376_limit == 0) A340376_build(1u << 16);
    while ((uint64_t)A340376_cnt < (uint64_t)n)
        A340376_build(A340376_limit * 4);
    return (seq_t)A340376_terms[(size_t)(n - 1)];
}
