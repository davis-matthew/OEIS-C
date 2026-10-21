/*
 * A259937  Concatenation of the numbers from 1 to n with numbers from n down
 *          to 1.
 * https://oeis.org/A259937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the numbers from 1 to n with * numbers
 *            from n down to 1. For each i, the digits of i are * appended by
 *            multiplying the current value by 10^digits(i) and adding i
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digits.h"

seq_t a(idx_t n) {
    seq_t res = 0;

    /* forward part: 1, 2, ..., n */
    for (idx_t i = 1; i <= n; ++i) {
        res = res * pow10((idx_t)digits(i)) + (seq_t)i;
    }

    /* reverse part: n, n-1, ..., 1 */
    for (idx_t i = n; i >= 1; --i) {
        res = res * pow10((idx_t)digits(i)) + (seq_t)i;
    }

    return res;
}
