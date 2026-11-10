/*
 * A369211  Numbers k such that A005361(k) = A005361(k+1).
 * https://oeis.org/A369211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k such that A005361(k) = A005361(k+1); computed by a
 *            sieve of A005361 with a cached term list
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9299 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A369211_terms = 0;
static uint64_t A369211_count = 0;

static void A369211_build(uint64_t L) {
    uint32_t *spf = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    uint32_t *rest = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    uint8_t *ex = (uint8_t *)calloc(L + 2, 1);
    uint64_t *f = (uint64_t *)calloc(L + 2, sizeof(uint64_t));
    uint64_t *t = (uint64_t *)malloc((L + 2) * sizeof(uint64_t));
    uint64_t cnt = 0, i, j;
    for (i = 2; i <= L + 1; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= L + 1; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    f[1] = 1;
    for (i = 2; i <= L + 1; i++) {
        uint64_t p = spf[i], m = i / p;
        if (m > 1 && spf[m] == p) {
            ex[i] = (uint8_t)(ex[m] + 1);
            rest[i] = rest[m];
        } else {
            ex[i] = 1;
            rest[i] = (uint32_t)m;
        }
        f[i] = (uint64_t)ex[i] * f[rest[i]];
    }
    for (i = 1; i <= L; i++)
        if (f[i] == f[i + 1]) t[cnt++] = i;
    free(spf); free(rest); free(ex); free(f);
    if (A369211_terms) free(A369211_terms);
    A369211_terms = t;
    A369211_count = cnt;
}

seq_t a(idx_t n) {
    if (n > A369211_count) {
        uint64_t L = 3 * (uint64_t)n + 1000;
        if (L < 2 * A369211_count * 3) L = 6 * A369211_count;
        for (;;) {
            A369211_build(L);
            if (A369211_count >= n) break;
            L *= 2;
        }
    }
    return (seq_t)A369211_terms[n - 1];
}
