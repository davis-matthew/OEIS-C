/*
 * A350960  a(n) = (A006285(n)-1)/2.
 * https://oeis.org/A350960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A006285(n+1)-1)/2, where A006285 lists odd numbers not
 *            of the form p + 2^k; computed by a sieve search
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *dp_terms = NULL;
static size_t dp_count = 0;
static uint64_t dp_limit = 0;

static void dp_build(uint64_t L) {
    unsigned char *comp = (unsigned char *)malloc((size_t)(L + 1));
    uint64_t i, j, m;
    size_t cap = 1024;
    memset(comp, 0, (size_t)(L + 1));
    comp[0] = 1;
    if (L >= 1) comp[1] = 1;
    for (i = 2; i * i <= L; i++)
        if (!comp[i])
            for (j = i * i; j <= L; j += i) comp[j] = 1;
    free(dp_terms);
    dp_terms = (uint64_t *)malloc(cap * sizeof(uint64_t));
    dp_count = 0;
    for (m = 1; m <= L; m += 2) {
        uint64_t p = 1;
        int ok = 1;
        while (p < m) {
            if (!comp[m - p]) { ok = 0; break; }
            p <<= 1;
        }
        if (ok) {
            if (dp_count == cap) {
                cap *= 2;
                dp_terms = (uint64_t *)realloc(dp_terms, cap * sizeof(uint64_t));
            }
            dp_terms[dp_count++] = m;
        }
    }
    free(comp);
    dp_limit = L;
}

static uint64_t A006285_a(uint64_t n) {
    while ((uint64_t)dp_count < n) {
        uint64_t L = n * 24 + 1000;
        if (L <= dp_limit) L = dp_limit * 2;
        dp_build(L);
    }
    return dp_terms[n - 1];
}

seq_t a(idx_t n) {
    uint64_t v = A006285_a((uint64_t)n + 1);
    return (seq_t)((v - 1) / 2);
}
