/*
 * A248422  Even integers concatenated with themselves.
 * https://oeis.org/A248422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2*n*10^ceil(log10(2*n)) + 2*n
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    seq_t v = (seq_t)n * 2;          /* even integer 2*n */
    int d = digit_len(v);           /* number of decimal digits of 2*n */
    seq_t p = pow10((idx_t)d);      /* 10^d */
    return v * p + v;               /* concatenate v with itself */
}
