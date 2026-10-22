/*
 * A337148  Concatenation of sum n+(n+1) and product n*(n+1) in decimal.
 * https://oeis.org/A337148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (2*n+1) * 10^b + n^2 + n , where b = digit_len(n^2 + n)
 * Range:     u8 n <= 2, u16 n <= 31, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;                  /* n as seq_t */
    seq_t sum  = sn * 2 + 1;                /* 2n+1 */
    seq_t prod = sn * (sn + 1);             /* n^2 + n */

    /* number of decimal digits of prod (handles 0 correctly) */
    int b = digit_len(prod);

    seq_t pow = pow10((idx_t)b);

    return sum * pow + prod;
}
