/*
 * A253445  a(n) = concatenation of n^2 with itself.
 * https://oeis.org/A253445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n^2 concatenated with itself (i.e., a(n) = n^2 * (1 +
 *            10^(digit_len(n^2)))
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 207, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* Computes a(n) = concatenation of n^2 with itself.
 * Works for any idx_t >= 1.
 */
seq_t a(idx_t n)
{
    seq_t sq   = (seq_t)n * (seq_t)n;          /* n^2 */
    int     d  = digit_len(sq);               /* number of decimal digits of n^2 */
    seq_t pow10_d = pow10((idx_t)d);          /* 10^d */
    return sq * (pow10_d + 1);                /* n^2 * (10^d + 1) == concat(n^2, n^2) */
}
