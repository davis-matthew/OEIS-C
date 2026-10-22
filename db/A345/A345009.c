/*
 * A345009  a(0) = 1; a(5*n) = a(n) - a(n-1), a(5*n+1) = a(5*n+2) = a(5*n+3) =
 *          a(5*n+4) = a(n).
 * https://oeis.org/A345009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(5*n) = a(n) - a(n-1); a(5*n+1) = a(5*n+2) = a(5*n+3)
 *            = a(5*n+4) = a(n) for all n ≥ 0 (Indices are zero‑based.)
 * Range:     i8 n <= 105, i16 n <= 105, i32 n <= 105, i64 n <= 105,
 *            i128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Returns the n‑th term of OEIS A345009. */
seq_t a(idx_t n)
{
    /* Base cases for n = 0,1,2,3,4: all equal to 1. */
    if (n < 5) {
        return 1;
    }

    /* If n is not a multiple of 5, the value equals that of n/5. */
    if (n % 5 != 0) {
        return a(n / 5);
    }

    /* n is a multiple of 5: a(5*n) = a(n) - a(n-1). */
    return a(n / 5) - a(n / 5 - 1);
}
