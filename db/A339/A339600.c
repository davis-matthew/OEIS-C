/*
 * A339600  a(n) = a(n-1) + 6*a(n-3) + 2*a(n-4), with a(0) = a(1) = 1, a(2) =
 *          3, a(3) = 6.
 * https://oeis.org/A339600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 6*a(n-3) + 2*a(n-4); Initial values: a(0)=1,
 *            a(1)=1, a(2)=3, a(3)=6
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 54, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate temporary array of length n+1 */
    size_t len = (size_t)n + 1;
    seq_t *t = (seq_t *)malloc(len * sizeof(seq_t));
    if (!t) exit(1);  /* allocation failed */

    /* Compute all terms up to n */
    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0)           t[i] = 1;
        else if (i == 1)      t[i] = 1;
        else if (i == 2)      t[i] = 3;
        else if (i == 3)      t[i] = 6;
        else                  t[i] = t[i-1] + 6 * t[i-3] + 2 * t[i-4];
    }

    seq_t result = t[n];
    free(t);
    return result;
}
