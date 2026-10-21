/*
 * A294934  Characteristic function for deficient numbers (A005100): a(n) = 1
 *          if A001065(n) < n, 0 otherwise.
 * https://oeis.org/A294934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if sigma(n) < 2*n else 0
 * Range:     u8 n <= 107, u16 n <= 9994, u32 n <= 9994, u64 n <= 9994,
 *            u128 n <= 9994, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* n == 0 is outside the OEIS offset; return 0 for safety. */
    if (n == 0) return 0;

    seq_t sigma = sigma_of(n);
    seq_t two_n = (seq_t)n * 2;
    return (sigma < two_n) ? 1 : 0;
}
