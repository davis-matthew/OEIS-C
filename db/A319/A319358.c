/*
 * A319358  a(n) = (10^n - 1)^9.
 * https://oeis.org/A319358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (10^n - 1)^9, with a(0) = 0
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   pow10, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t t = pow10(n);   /* t = 10^n */
    t = t - (seq_t)1;     /* t = 10^n - 1 */
    return pow_seq(t, 9); /* t^9 */
}
