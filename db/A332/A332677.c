/*
 * A332677  a(n) is the number of ways n can be obtained by adding some digit
 *          of a number k to k.
 * https://oeis.org/A332677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of integers k such that k + d = n where d is a
 *            digit of k (decimal)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (int d = 0; d <= 9; ++d) {
        if (n < (idx_t)d) continue;           /* k would be negative */
        seq_t k = (seq_t)n - (seq_t)d;

        int found = 0;
        if (k == 0) {
            if (d == 0) found = 1;             /* 0 contains the digit 0 */
        } else {
            seq_t tmp = k;
            while (tmp > 0) {
                if ((tmp % 10) == (seq_t)d) {
                    found = 1;
                    break;
                }
                tmp /= 10;
            }
        }
        if (found) ++count;
    }
    return count;
}
