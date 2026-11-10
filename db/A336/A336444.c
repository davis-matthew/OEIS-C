/*
 * A336444  Numbers m such that k + A005361(k) <= m for all k < m.
 * https://oeis.org/A336444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m with max_{k<m} (k + A005361(k)) <= m; computed by
 *            a sieve scan with a running maximum
 * Range:     u8 n <= 91, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9659 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *A336444_terms = 0;
static uint64_t A336444_cnt = 0;
static uint64_t A336444_lim = 0;

static void A336444_build(uint64_t L) {
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 2, sizeof(uint32_t));
    uint64_t *out = (uint64_t *)malloc(((size_t)L + 2) * sizeof(uint64_t));
    uint64_t i, j, cnt = 0, mx = 0;
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (i = 1; i <= L; i++) {
        if (mx <= i) out[cnt++] = i;
        {
            uint64_t f = 1, m = i;
            while (m > 1) {
                uint32_t p = spf[m];
                uint64_t e = 0;
                while (m % p == 0) { m /= p; e++; }
                f *= e;
            }
            if (i + f > mx) mx = i + f;
        }
    }
    free(spf);
    if (A336444_terms) free(A336444_terms);
    A336444_terms = out;
    A336444_cnt = cnt;
    A336444_lim = L;
}

seq_t a(idx_t n) {
    while (n > A336444_cnt || A336444_terms == 0) {
        uint64_t L = (uint64_t)n * 4 + 100;
        if (L < 2 * A336444_lim) L = 2 * A336444_lim;
        A336444_build(L);
    }
    return (seq_t)A336444_terms[n - 1];
}
