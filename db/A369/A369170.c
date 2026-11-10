/*
 * A369170  Numbers k such that A000005(k) <= A000688(k).
 * https://oeis.org/A369170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k such that A000005(k) <= A000688(k); found by an
 *            spf sieve with doubling limit, A000688 = Product_{p^e||k}
 *            partitions(e)
 * Range:     u8 n <= 7, u16 n <= 236, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9862 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *A369170_terms = 0;
static size_t A369170_cnt = 0;
static uint64_t A369170_limit = 0;

static void A369170_build(uint64_t L)
{
    uint64_t part[40];
    uint32_t *spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint32_t *out = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(L + 1));
    size_t cnt = 0;
    uint64_t i, j;
    int p, q;

    for (i = 0; i < 40; i++) part[i] = 0;
    part[0] = 1;
    for (p = 1; p < 40; p++)
        for (q = p; q < 40; q++)
            part[q] += part[q - p];

    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i <= L / i)
                for (j = i * i; j <= L; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    out[cnt++] = 1; /* k = 1: d=1, groups=1 */
    for (i = 2; i <= L; i++) {
        uint64_t m = i, d = 1, g = 1;
        while (m > 1) {
            uint32_t pr = spf[m];
            int e = 0;
            while (m % pr == 0) { m /= pr; e++; }
            d *= (uint64_t)(e + 1);
            g *= part[e];
        }
        if (d <= g) out[cnt++] = (uint32_t)i;
    }
    free(spf);
    if (A369170_terms) free(A369170_terms);
    A369170_terms = out;
    A369170_cnt = cnt;
    A369170_limit = L;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A369170_limit == 0) A369170_build(1 << 16);
    while ((uint64_t)n > A369170_cnt)
        A369170_build(A369170_limit * 2);
    return (seq_t)A369170_terms[n - 1];
}
