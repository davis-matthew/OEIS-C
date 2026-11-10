/*
 * A274460  Values of A003327(n) that are the sum of 4 but no fewer nonzero
 *          squares.
 * https://oeis.org/A274460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th number m that is a sum of 4 positive cubes (A003327)
 *            and of the form 4^k*(8j+7), i.e. not a sum of 3 squares;
 *            computed by a cube sieve with a doubling limit
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *a274460_terms = NULL;
static idx_t a274460_count = 0;
static uint64_t a274460_limit = 0;

static int a274460_needs4(uint64_t m) {
    if (m == 0) return 0;
    while ((m & 3) == 0) m >>= 2;
    return (m & 7) == 7;
}

static void a274460_build(uint64_t L) {
    unsigned char *mark = (unsigned char *)calloc((size_t)L, 1);
    uint64_t a, b, c, d, cnt = 0, m;
    if (!mark) return;
    for (a = 1; 4 * a * a * a < L; a++) {
        uint64_t s1 = a * a * a;
        for (b = a; s1 + 3 * b * b * b < L; b++) {
            uint64_t s2 = s1 + b * b * b;
            for (c = b; s2 + 2 * c * c * c < L; c++) {
                uint64_t s3 = s2 + c * c * c;
                for (d = c; s3 + d * d * d < L; d++)
                    mark[s3 + d * d * d] = 1;
            }
        }
    }
    for (m = 0; m < L; m++) if (mark[m] && a274460_needs4(m)) cnt++;
    free(a274460_terms);
    a274460_terms = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    a274460_count = 0;
    if (a274460_terms) {
        for (m = 0; m < L; m++)
            if (mark[m] && a274460_needs4(m)) a274460_terms[a274460_count++] = m;
    }
    a274460_limit = L;
    free(mark);
}

seq_t a(idx_t n) {
    uint64_t L = a274460_limit ? a274460_limit : 1024;
    if (n == 0) return (seq_t)0;
    while (a274460_count < n) {
        if (a274460_limit) L *= 2;
        a274460_build(L);
        if (!a274460_terms) return (seq_t)0;
    }
    return (seq_t)a274460_terms[n - 1];
}
