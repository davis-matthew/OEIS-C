/*
 * A323426  a(n) = decimal concatenation of n (once), n-1 (twice), n-2 (3
 *          times), ..., 1 (n times).
 * https://oeis.org/A323426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, less
 * Formula:   a(n) = concatenation of i repeated (n-i+1) times for i = 1..n
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   digits_of, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digits_of.h"

seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    for (idx_t i = n; i > 0; --i)
    {
        idx_t d = digits_of(i);                 /* number of decimal digits of i */
        seq_t base = pow10(d);                  /* 10^d */
        idx_t m = n - i + 1;                    /* repetition count for i */
        idx_t exp = d * m;                      /* exponent for 10^(d*m) */
        seq_t powlen_m = pow10(exp);            /* 10^(d*m) */
        seq_t numerator = powlen_m - (seq_t)1;   /* 10^(d*m) - 1 */
        seq_t denom = base - (seq_t)1;           /* 10^d - 1 */
        seq_t quotient = numerator / denom;     /* (10^(d*m)-1)/(10^d-1) */
        seq_t block = quotient * (seq_t)i;      /* repeated i's value */
        result = result * powlen_m + block;     /* shift left and add block */
    }
    return result;
}
