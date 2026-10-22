/*
 * A348023  a(n) = [x^n] Product_{k=0..2*n-1} (x + (-1)^k * k).
 * https://oeis.org/A348023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^n] Π_{k=0}^{2n-1} (x + (-1)^k * k)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case n = 0: the empty product equals 1 */
    if (n == 0)
        return (seq_t)1;

    /* Workspace: e[j] will hold the j‑th elementary symmetric sum
       of the numbers (-1)^k * k for k = 1 .. 2n-1.  We need indices
       0..n. */
    size_t sz = (size_t)n + 1;
    seq_t *e = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!e)
        abort();                    /* memory allocation failure */

    e[0] = (seq_t)1;
    for (size_t i = 1; i < sz; ++i)
        e[i] = (seq_t)0;

    /* Process each factor (x + (-1)^k * k) with k = 1 .. 2n-1 */
    for (idx_t k = 1; k <= 2 * n - 1; ++k) {
        seq_t t = (k & 1) ? -(seq_t)k : (seq_t)k;   /* (-1)^k * k */
        idx_t jlim = k < n ? k : n;                 /* min(k,n) */
        for (idx_t j = jlim; j > 0; --j) {
            e[j] = e[j] + t * e[j - 1];
        }
    }

    seq_t result = e[n];
    free(e);
    return result;
}
