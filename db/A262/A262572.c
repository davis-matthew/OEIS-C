/*
 * A262572  Concatenation of the numbers from 1 to n but omitting 2.
 * https://oeis.org/A262572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of integers from 1 to n+1, excluding the
 *            number 2. * The value is built incrementally: a = a *
 *            10^digits(k) + k (k ≠ 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digits.h"   /* int digits(idx_t x); */
#include "../utils/pow10.h"    /* seq_t pow10(idx_t e); */

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return 0;          /* undefined for n < 1 */

    seq_t result = 0;

    /* concatenate numbers 1 .. n+1, but skip the number 2 */
    for (idx_t k = (idx_t)1; k <= n + (idx_t)1; ++k) {
        if (k == (idx_t)2) continue;     /* omit the number 2 */

        int d = digits(k);               /* number of decimal digits of k */
        seq_t mult = pow10((idx_t)d);    /* 10^d as seq_t            */
        result = result * mult + (seq_t)k;
    }

    return result;
}
