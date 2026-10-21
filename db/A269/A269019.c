/*
 * A269019  a(n) = 2^n + 2*(-1)^n - 1.
 * https://oeis.org/A269019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2^n + 2*(-1)^n - 1
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"  /* provides seq_t pow2(idx_t exp) */

/* Compute a(n) for the OEIS sequence A269019 */
seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t p = pow2(n);

    /* compute 2*(-1)^n */
    seq_t s = (seq_t)2;
    if ((n & 1))   /* odd n => (-1)^n = -1 */
        s = -s;

    /* a(n) = 2^n + 2*(-1)^n - 1 */
    return p + s - (seq_t)1;
}
