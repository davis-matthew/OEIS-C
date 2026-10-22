/*
 * A345318  Median absolute deviation of {2*k^2 | k=1..n}.
 * https://oeis.org/A345318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = median{ |2*k^2 - median{2*j^2}| : 1 <= k <= n }
 * Range:     u8 n <= 11, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Comparator for qsort on seq_t values.  Works for all integer sizes. */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a, *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    return *pa > *pb;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1 */

    /* ---- median of { 2*k^2 | 1 <= k <= n } ------------------------------------ */
    idx_t mid = n / 2;                    /* floor(n/2) */
    seq_t median_val;
    if (n & 1) {                          /* n odd : middle element at k = mid+1 */
        seq_t k = (seq_t)(mid + 1);
        median_val = k * k * 2;
    } else {                              /* n even : average of two middle elements */
        seq_t k1 = (seq_t)mid;
        seq_t k2 = (seq_t)(mid + 1);
        median_val = k1 * k1 + k2 * k2;   /* (2*k1^2 + 2*k2^2)/2 = k1^2 + k2^2 */
    }

    /* ---- absolute deviations from the median --------------------------------- */
    seq_t *devs = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!devs) return 0;                  /* allocation failure */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t vi = (seq_t)i * (seq_t)i * 2;
        devs[i - 1] = vi >= median_val ? vi - median_val : median_val - vi;
    }

    /* ---- median of the deviations -------------------------------------------- */
    qsort(devs, (size_t)n, sizeof(seq_t), cmp_seq_t);

    seq_t result;
    if (n & 1) {                          /* n odd */
        result = devs[n / 2];
    } else {                              /* n even */
        result = (devs[n / 2 - 1] + devs[n / 2]) / 2;
    }

    free(devs);
    return result;
}
