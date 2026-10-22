/*
 * A302774  a(n) is the position of the first term in A303762 that has
 *          prime(n) as one of its prime factors.
 * https://oeis.org/A302774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{i=1}^{n-1} d(i) where d(1)=1, d(2)=2, d(3)=4,
 *            d(4)=7, and for i>=5: if i is odd: d(i) = 3*d(i-2) + 4 if i is
 *            even: d(i) = 3*d(i-2) - 2 This recurrence reproduces the
 *            differences of A302774
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    idx_t len = n - 1;                    /* number of differences to sum */
    seq_t *diff = (seq_t *)malloc((len + 1) * sizeof(seq_t));
    if (!diff) return (seq_t)0;           /* allocation failed */

    /* base differences */
    diff[1] = (seq_t)1;
    if (len >= 2) diff[2] = (seq_t)2;
    if (len >= 3) diff[3] = (seq_t)4;
    if (len >= 4) diff[4] = (seq_t)7;

    /* recurrence for remaining differences */
    for (idx_t i = 5; i <= len; ++i) {
        if ((i & 1) == 1) { /* odd index */
            diff[i] = diff[i-2] * (seq_t)3 + (seq_t)4;
        } else {            /* even index */
            diff[i] = diff[i-2] * (seq_t)3 - (seq_t)2;
        }
    }

    /* compute a(n) */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= len; ++i) {
        result += diff[i];
    }

    free(diff);
    return result;
}
