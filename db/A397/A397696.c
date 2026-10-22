/*
 * A397696  Number of adjacent differences less than 1/n in the sorted list
 *          with multiplicity of the fractional parts of sqrt(k) for n <= k <=
 *          2n.
 * https://oeis.org/A397696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of adjacent gaps < 1/n in the sorted list of
 *            fractional parts { frac( sqrt(k) ) | n <= k <= 2n }
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   cmp_double
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>   /* for sqrt in double precision */
#include "../utils/cmp_double.h"

/* Return a(n).  n >= 1. */
seq_t a(idx_t n)
{
    /* There are n+1 values in the range [n,2n]. */
    size_t len = (size_t)n + 1;
    double *vals = (double *)malloc(len * sizeof(double));
    if (!vals) return (seq_t)0;  /* OOM guard, unlikely in test harness */

    /* Fill the array with the fractional parts of sqrt(k). */
    for (idx_t k = n; k <= (idx_t)(2 * n); ++k) {
        double sq = sqrt((double)k);
        /* floor(sq) works because sq is non‑negative */
        double frac = sq - (double)((unsigned long long)sq);
        vals[k - n] = frac;
    }

    /* Sort the fractional parts. */
    qsort(vals, len, sizeof(double), cmp_double);

    /* Count adjacent differences smaller than 1/n. */
    seq_t count = 0;
    double thresh = 1.0 / (double)n;
    for (size_t i = 0; i + 1 < len; ++i) {
        if ((vals[i + 1] - vals[i]) < thresh) {
            count++;
        }
    }

    free(vals);
    return count;
}
