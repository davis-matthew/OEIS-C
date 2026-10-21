/*
 * A273092  a(n) = 2^n - 1 written backwards.
 * https://oeis.org/A273092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = reverse_decimal(2^n-1)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    seq_t v = pow2(n);     /* compute 2^n as seq_t */
    v -= (seq_t)1;        /* subtract 1 to get 2^n - 1 */
    return reverse_decimal(v);  /* reverse decimal digits */
}
