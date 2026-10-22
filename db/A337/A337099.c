/*
 * A337099  Largest positive number using exactly n segments on a calculator
 *          display (when '6' and '7' are represented using 6 resp. 3
 *          segments).
 * https://oeis.org/A337099
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (10^(n/2) - 1)/9 for even n * a(n) = ((10^((n-1)/2) -
 *            1)/9) + 6*10^((n-3)/2) for odd n * Offset: 2 (a(2) = 1 is the
 *            first defined term)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 2.  Below the offset the
     * return value is undefined (here 0) because it is not used by the
     * tests. */
    if (n < 2)
        return 0;

    if ((n & 1) == 0) {                /* even n */
        idx_t m = n >> 1;              /* m = n/2 */
        seq_t p = pow10(m);            /* 10^m */
        return (p - 1) / 9;            /* (10^m - 1)/9 */
    } else {                           /* odd n */
        idx_t m = (n - 1) >> 1;        /* m = (n-1)/2 */
        seq_t p = pow10(m);            /* 10^m  (m >= 1) */
        seq_t base = (p - 1) / 9;      /* (10^m - 1)/9 */
        seq_t t = pow10(m - 1);        /* 10^(m-1) */
        return base + (seq_t)6 * t;    /* + 6 * 10^(m-1) */
    }
}
