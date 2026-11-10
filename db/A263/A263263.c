/*
 * A263263  Row 3 of A263255.
 * https://oeis.org/A263263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve/search. Tree with parent(m) = A049820(m) = m - d(m); the
 *            trunk is the set of nodes having descendants far above the
 *            bound. a(n) = n-th number whose distance to the trunk is 3
 * Range:     u8 n <= 19, u16 n <= 2047, u32 n <= 2047, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Validated: standard, pass, 80 terms through n = 1982 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A263263_terms = 0;
static uint64_t A263263_count = 0;

static void A263263_build(uint64_t L)
{
    uint32_t *d = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    unsigned char *mark = (unsigned char *)calloc((size_t)L + 1, 1);
    uint64_t X = L / 32;
    uint64_t i, j, z, cnt = 0;
    if (!d || !mark) { free(d); free(mark); return; }
    for (i = 1; i <= L; i++)
        for (j = i; j <= L; j += i)
            d[j]++;
    mark[0] = 1;
    for (z = L / 2; z <= L; z++) {
        uint64_t y = z;
        while (!mark[y]) {
            mark[y] = 1;
            y = y - d[y];
        }
    }
    free(A263263_terms);
    A263263_terms = (uint32_t *)malloc((size_t)(X + 1) * sizeof(uint32_t));
    if (A263263_terms) {
        for (i = 1; i <= X; i++) {
            uint64_t y = i;
            int steps = 0;
            while (!mark[y] && steps < 4) {
                y = y - d[y];
                steps++;
            }
            if (steps == 3 && mark[y])
                A263263_terms[cnt++] = (uint32_t)i;
        }
    }
    A263263_count = cnt;
    free(d);
    free(mark);
}

seq_t a(idx_t n)
{
    static uint64_t L = 0;
    if (n < 1) return 0;
    if (L == 0) {
        L = (uint64_t)1 << 21;
        A263263_build(L);
    }
    while (A263263_count < (uint64_t)n && L < ((uint64_t)1 << 28)) {
        L *= 2;
        A263263_build(L);
    }
    if ((uint64_t)n > A263263_count) return 0;
    return (seq_t)A263263_terms[n - 1];
}
