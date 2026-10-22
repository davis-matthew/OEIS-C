/*
 * A334417  a(n) is the palindrome equal to A334416(n) divided by its sum of
 *          digits.
 * https://oeis.org/A334417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A334416(n) / A007953(A334416(n)) where A334416(n) is the
 *            nth positive integer m such that m / sum_of_digits(m) is an
 *            integer palindrome. This implementation enumerates m in
 *            increasing order, tests the divisibility and palindrome
 *            condition, and returns the nth resulting quotient
 * Range:     u8 n <= 27, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   digit_sum, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n=0, return 0 */
    idx_t count = 0;
    idx_t m = 1;

    while (1) {
        seq_t d = digit_sum((seq_t)m);  /* sum of digits of m */
        /* d will be small (<= 9 * number_of_digits(m)), so cast to idx_t safely */
        if (m % (idx_t)d == 0) {
            seq_t q = (seq_t)(m / (idx_t)d);  /* quotient */
            if (is_palindrome(q, 10)) {
                ++count;
                if (count == n) return q;
            }
        }
        ++m;
    }
}
