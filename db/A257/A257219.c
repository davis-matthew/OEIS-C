/*
 * A257219  Numbers that have at least one divisor containing the digit 2 in
 *          base 10.
 * https://oeis.org/A257219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer that has at least one divisor containing
 *            the digit 2 in base 10. This includes all even numbers, all
 *            numbers that themselves contain the digit 2, and odd multiples
 *            of numbers that contain the digit 2. The implementation
 *            enumerates integers, testing the described condition...
 * Range:     u8 n <= 178, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains_digit_2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/contains_digit_2.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t num   = 2;               /* the first term is 2 */

    for (;;) {
        seq_t value = (seq_t)num;
        bool ok = false;

        /* Even numbers always qualify. */
        if ((num & 1) == 0) {
            ok = true;
        }
        /* Numbers that contain the digit 2 in themselves. */
        else if (contains_digit_2(value)) {
            ok = true;
        }
        /* Check odd numbers that do not contain the digit 2. */
        else {
            for (idx_t d = 1; d * d <= num; ++d) {
                if (num % d == 0) {
                    /* Skip divisor 1 (it cannot contain 2). */
                    if (d != 1 && contains_digit_2((seq_t)d)) {
                        ok = true;
                        break;
                    }
                    idx_t other = num / d;
                    /* Skip the number itself. */
                    if (other != d && other != num && contains_digit_2((seq_t)other)) {
                        ok = true;
                        break;
                    }
                }
            }
        }

        if (ok) {
            ++count;
            if (count == n)
                return value;
        }
        ++num;
    }
}
