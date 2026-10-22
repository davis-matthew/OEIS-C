/*
 * A319070  a(n) is the area of the surface made of the rectangles with
 *          vertices (d, n/d), (D, n/d), (D, n/D), (d, n/D) for all (d, D),
 *          pair of consecutive divisors of n.
 * https://oeis.org/A319070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Σ_{consecutive divisors d < D} n·(D‑d)²/(d·D)
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 4116, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/divisor_count_u64.h"

static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0 */

    /* prime factorisation */
    size_t fp_cnt = 0;
    uint64_t *fp = NULL, *fpex = NULL;
    fps: free(fp); free(fpex);   /* macro to free if allocated earlier */

    size_t count = 0;
    fp = (uint64_t*)malloc(64 * sizeof(uint64_t));
    fpex = (uint64_t*)malloc(64 * sizeof(uint64_t));
    factor_u64((uint64_t)n, fp, fpex, &count);

    /* number of divisors */
    size_t dcnt = divisor_count_u64((uint64_t)n);
    seq_t *divs = (seq_t*)malloc(dcnt * sizeof(seq_t));
    size_t sz = 1;
    divs[0] = 1;

    for (size_t i = 0; i < count; ++i) {
        seq_t prime = (seq_t)fp[i];
        seq_t pow = prime;
        size_t oldsz = sz;
        for (size_t e = 1; e <= fpex[i]; ++e) {
            for (size_t j = 0; j < oldsz; ++j)
                divs[sz++] = divs[j] * pow;
            pow = pow * prime;
        }
    }

    /* sort divisors */
    qsort(divs, dcnt, sizeof(seq_t), cmp_seq);

    /* compute area sum */
    seq_t total = 0;
    for (size_t i = 0; i + 1 < dcnt; ++i) {
        seq_t d = divs[i];
        seq_t D = divs[i + 1];
        seq_t diff = (seq_t)((uint64_t)D - (uint64_t)d);
        seq_t num  = (seq_t)n * diff * diff;
        seq_t denom = d * D;
        total += num / denom;
    }

    free(divs);
    free(fp);
    free(fpex);
    return total;
}
