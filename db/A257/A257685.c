/*
 * A257685  Left inverse for injection A255411: a(0) = 0, after which, if n =
 *          A255411(k) for some k, then a(n) = k, otherwise a(n) = 0.
 * https://oeis.org/A257685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1-A257680(n)) * A257684(n); i.e. if the factorial base
 *            representation of n has no digit 1, drop the last digit and
 *            decrement the nonzero digits, otherwise a(n) = 0
 * Range:     u8 n <= 2249, u16 n <= 10080, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 137 terms through n = 9825 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t fact = 1;      /* (i-1)! for the digit at position i */
    idx_t rem = n;
    idx_t base = 2;      /* divisor for digit position 1 */
    int pos = 1;

    while (rem > 0) {
        idx_t d = rem % base;
        rem /= base;
        if (d == 1)
            return 0;
        if (pos >= 2) {
            if (d > 0)
                result += (seq_t)(d - 1) * fact;
            fact *= (seq_t)(pos);   /* becomes pos! for next position */
        }
        ++pos;
        ++base;
    }
    return result;
}
