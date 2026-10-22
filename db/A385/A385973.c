/*
 * A385973  The hypotenuses of the triangles defined in A365577.
 * https://oeis.org/A385973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   pow2_repeated
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A385973
   Formula: a(n) = 2^(2^n + 1) - 2^(2^(n-1) + 1) + 1   (n >= 1)
   This follows from the definition of the hypotenuses in A365577.
   For n = 1 the formula yields a(1) = 5, matching the first term. */

#include <stdint.h>
#include "../utils/pow2_repeated.h"

seq_t a(idx_t n)
{
    /* Compute 2^(2^n + 1) */
    seq_t val_full = pow2_repeated(n);
    val_full = val_full * 2;     /* multiply by 2 -> +1 in exponent */

    /* Compute 2^(2^(n-1) + 1) */
    seq_t val_prev = pow2_repeated(n > 0 ? n - 1 : 0);
    val_prev = val_prev * 2;     /* multiply by 2 -> +1 in exponent */

    /* a(n) = val_full - val_prev + 1 */
    seq_t result = val_full - val_prev + 1;
    return result;
}
