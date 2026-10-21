/*
 * A248038  3n concatenated with itself.
 * https://oeis.org/A248038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenate(3*n, 3*n) in base‑10, i.e. a(n) = 3*n *
 *            10^digits(3*n) + 3*n
 * Range:     u8 n <= 3, u16 n <= 33, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* compute 3*n in the target integer type */
    seq_t three_n = (seq_t)n * (seq_t)3;

    /* number of decimal digits of 3*n */
    int d = digit_len(three_n);

    /* 10^d as a seq_t */
    seq_t pow10d = pow10((idx_t)d);

    /* concatenate by multiplication and addition */
    return three_n * pow10d + three_n;
}
