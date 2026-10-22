/*
 * A325675  First term of n-th difference sequence of (floor(k*r)), r =
 *          -sqrt(8), k >= 0.
 * https://oeis.org/A325675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Δ^n floor(k*(-sqrt(8))) evaluated at k = 0 (n-th forward
 *            difference of the sequence floor(k*r), r = -sqrt(8), at index 0)
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 34, i64 n <= 67, i128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of range, return 0 */
    idx_t size = n + 1;

    seq_t *b = (seq_t*)malloc(size * sizeof(seq_t));
    if (!b) return 0;                /* allocation failed */

    /* pre‑compute floor(k * sqrt(8)) for k = 0 .. n */
    const long double sqrt8 = sqrtl(8.0L);
    const long double eps   = 1e-12L;

    b[0] = 0;                        /* floor(0) = 0 */

    for (idx_t k = 1; k < size; ++k) {
        long double prod = (long double)k * sqrt8;
        idx_t floor_val = (idx_t)(prod + eps);        /* floor(k*sqrt8) */
        b[k] = -((seq_t)floor_val + 1);              /* -ceil(k*sqrt8)   */
    }

    /* compute forward differences n times, keeping only the first element */
    for (idx_t d = 0; d < n; ++d) {
        idx_t limit = size - 1 - d;
        for (idx_t i = 0; i < limit; ++i) {
            b[i] = b[i + 1] - b[i];
        }
    }

    seq_t result = b[0];
    free(b);
    return result;
}
