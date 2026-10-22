/*
 * A396584  a(n) = (4 - 3*(-1)^n) * 5^floor(n/2).
 * https://oeis.org/A396584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4 - 3*(-1)^n) * 5^floor(n/2)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* 5^floor(n/2) */
    idx_t e = n >> 1;                 /* floor(n/2) */
    seq_t pow5 = seq_pow((seq_t)5, e);/* pow(5, e) */

    /* factor: 1 for even n, 7 for odd n */
    seq_t factor = (n & 1) ? (seq_t)7 : (seq_t)1;

    return seq_mul(pow5, factor);
}
