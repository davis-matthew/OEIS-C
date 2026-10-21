/*
 * A248555  a(n) is the concatenation of 3n-2, 3n and 3n-1.
 * https://oeis.org/A248555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = concat(3n-2, 3n, 3n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 333, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Concatenates the decimal representations of 3n-2, 3n, and 3n-1. */
#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;          /* convert n to seq_t to avoid overflow */
    seq_t a1 = tn * (seq_t)3 - (seq_t)2; /* 3n-2 */
    seq_t b1 = tn * (seq_t)3;           /* 3n   */
    seq_t c1 = tn * (seq_t)3 - (seq_t)1;/* 3n-1 */

    int len_b = digit_len(b1);         /* number of decimal digits of 3n */
    int len_c = digit_len(c1);         /* number of decimal digits of 3n-1 */

    seq_t res = a1 * pow10((idx_t)(len_b + len_c))
              + b1 * pow10((idx_t)(len_c))
              + c1;
    return res;
}
