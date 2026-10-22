/*
 * A323618  Expansion of e.g.f. (1 + x)*log(1 + x)*(2 + log(1 + x))/2.
 * https://oeis.org/A323618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 14, i64 n <= 22, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (5 - 2*n) * a(n-1) - (n-3)^2 * a(n-2)  for n >= 4
 * Initial values: a(0) = 0, a(1) = 1, a(2) = 2, a(3) = -1
 *
 * This implementation calculates the sequence using the linear
 * recurrence, in O(n) time and O(n) space for each call.
 * It works for any integer type seq_t (signed) and for idx_t
 * (unsigned 64‑bit index) provided by the build.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle trivial cases */
    if (n == 0)
        return 0;
    /* allocate array for intermediate values */
    seq_t *v = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!v)
        abort();          /* allocation failed -- terminate */
    v[0] = 0;
    if (n >= 1)
        v[1] = 1;
    if (n >= 2)
        v[2] = 2;
    if (n >= 3)
        v[3] = -1;
    for (idx_t i = 4; i <= n; ++i) {
        seq_t factor1 = (seq_t)(5 - 2 * (seq_t)i);
        seq_t t = (seq_t)(i - 3);
        seq_t factor2 = t * t;
        v[i] = factor1 * v[i - 1] - factor2 * v[i - 2];
    }
    seq_t result = v[n];
    free(v);
    return result;
}
