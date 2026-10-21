/*
 * A249166  Odd integers concatenated with themselves.
 * https://oeis.org/A249166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(2n-1, 2n-1) (concatenation in base 10)
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* odd integer 2n-1 */
    seq_t odd = (seq_t)n * (seq_t)2 - (seq_t)1;

    /* number of decimal digits of odd */
    int d = digit_len(odd);

    /* 10^d as seq_t */
    seq_t pow10d = pow10((idx_t)d);

    /* concatenation: odd * 10^d + odd */
    return odd * pow10d + odd;
}
