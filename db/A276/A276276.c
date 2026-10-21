/*
 * A276276  a(n) = a(n-2)+a(n-3) with a(1)=2 a(2)=1 a(3)=0.
 * https://oeis.org/A276276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-3), a(1)=2, a(2)=1, a(3)=0
 * Range:     u8 n <= 22, u16 n <= 41, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1) return 2;
    if (n == 2) return 1;
    if (n == 3) return 0;

    /* Allocate an array large enough to hold indices 1..n */
    size_t sz = (size_t)(n + 1);
    seq_t *values = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!values)  /* allocation failed – return 0 as a fallback */
        return 0;

    values[1] = 2;
    values[2] = 1;
    values[3] = 0;

    for (idx_t i = 4; i <= n; ++i)
        values[i] = values[i - 2] + values[i - 3];

    seq_t result = values[n];
    free(values);
    return result;
}
