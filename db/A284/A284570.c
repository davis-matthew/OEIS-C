/*
 * A284570  a(n) = A000005((n+1)^2) - A000005(n^2).
 * https://oeis.org/A284570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A000005((n+1)^2) - A000005(n^2)
 * Range:     i8 n <= 418, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   tau_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/tau_square.h"

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1) */
    uint64_t nn = (uint64_t)n;
    seq_t d_n   = tau_square(nn);
    seq_t d_np1 = tau_square(nn + 1);
    return d_np1 - d_n;
}
