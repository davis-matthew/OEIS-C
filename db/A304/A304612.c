/*
 * A304612  a(n) = 75*2^n - 38.
 * https://oeis.org/A304612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 75*2^n - 38
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n using the utility function. */
    seq_t term = pow2(n);

    /* Multiply by 75 and subtract 38.  all operations stay within seq_t. */
    term *= (seq_t)75;
    term -= (seq_t)38;

    return term;
}
