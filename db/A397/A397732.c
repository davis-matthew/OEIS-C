/*
 * A397732  a(n) is the number of distinct triples of triangles chosen from
 *          n^2 unit triangles of a triangular grid up to rotation such that
 *          no two triangles share a common point.
 * https://oeis.org/A397732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 21*a(n-3) − 21*a(n-4) + 21*a(n-5)
 *            − 21*a(n-6) + 15*a(n-7) − 6*a(n-8) + a(n-9) (n ≥ 12) Initial
 *            values (offset 1): a(1)=0, a(2)=0, a(3)=2, a(4)=30, a(5)=228,
 *            a(6)=1016, a(7)=3270, a(8)=8538, a(9)=19304, a(10)=39288,
 *            a(11)=73782
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 65, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* pre‑computed first 11 terms */
    static const seq_t init[11] = {
        (seq_t)0,   (seq_t)0,   (seq_t)2,
        (seq_t)30,  (seq_t)228, (seq_t)1016,
        (seq_t)3270,(seq_t)8538,(seq_t)19304,
        (seq_t)39288,(seq_t)73782
    };

    if (n <= 11) return init[n-1];

    /* allocate an array for 1 … n (index 0 is unused) */
    size_t m = (size_t)n;
    seq_t *seq = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    if (!seq) abort();          /* minimal error handling */

    /* initialise the first eleven terms */
    for (idx_t i = 1; i <= 11; ++i)
        seq[(size_t)i] = init[i-1];

    /* compute terms from 12 up to n */
    for (size_t i = 12; i <= m; ++i) {
        /* build the term using the recurrence */
        seq_t t = (seq_t)6 * seq[i-1]       /* a(n-1) */
                + (seq_t)21 * seq[i-3]      /* a(n-3) */
                + (seq_t)21 * seq[i-5]      /* a(n-5) */
                + (seq_t)15 * seq[i-7]      /* a(n-7) */
                + seq[i-9];                 /* a(n-9) */
        t -= (seq_t)15 * seq[i-2]          /* a(n-2) */
            + (seq_t)21 * seq[i-4]         /* a(n-4) */
            + (seq_t)21 * seq[i-6]         /* a(n-6) */
            + (seq_t)6  * seq[i-8];        /* a(n-8) */
        seq[i] = t;
    }

    seq_t result = seq[m];
    free(seq);
    return result;
}
