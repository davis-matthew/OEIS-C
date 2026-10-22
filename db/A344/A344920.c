/*
 * A344920  The Worpitzky transform of the squares.
 * https://oeis.org/A344920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^(n+1)*(3 - 2^(n+1)) for n >= 1 a(0) = 0
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Return the n-th term of A344920. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute 2^(n+1) as a seq_t via the supplied pow2 helper. */
    seq_t two_pow = pow2((idx_t)(n + 1));

    /* Compute (3 - 2^(n+1)). */
    seq_t t = (seq_t)3 - two_pow;

    /* Determine the sign:  (-1)^(n+1). */
    if (((n + 1) & 1ULL) == 0)          /* even exponent -> +1  */
        return t;                       /* a(n) =  (3 - 2^(n+1)) */
    else
        return -t;                      /* a(n) = -(3 - 2^(n+1)) */
}
