/*
 * A290148  a(n) is the integer resulting from the concatenation of the unit
 *          digit of n-1 to the digits of n without its own unit digit.
 * https://oeis.org/A290148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = (n-1 mod 10) * 10^digits(floor(n/10)) + floor(n/10) with
 *            the convention that for floor(n/10)=0 the suffix is omitted
 *            (i.e., a(n)=n-1 for 1≤n≤9)
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 9747, u64 n <= 9747,
 *            u128 n <= 9747, big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digits.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* n >= 1 */
    seq_t d = (seq_t)(n - 1) % 10;      /* last digit of n-1   */
    seq_t t = (seq_t)n / 10;            /* n without its last digit */

    if (t == 0)                         /* n < 10              */
        return d;                       /* a(n)=n-1            */

    int len = digits((idx_t)t);         /* number of digits of t */
    seq_t pw = pow10((idx_t)len);       /* 10^len             */

    return d * pw + t;
}
