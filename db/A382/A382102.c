/*
 * A382102  Remove all occurrences of a digit from n such that the resulting
 *          number, formed by the remaining digits in their original order, is
 *          as small as possible. If no digits remain, a(n)=0.
 * https://oeis.org/A382102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 2565, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = min_{d=0..9} (remove all occurrences of digit d from the
 *                 decimal representation of n and interpret the remaining
 *                 digits in their original order).  If all digits are
 *                 removed, a(n) = 0.
 */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Highest power of 10 <= n */
    idx_t pow10 = 1;
    while (pow10 <= n / 10) {
        pow10 *= 10;
    }

    seq_t best = 0;
    bool first = true;

    for (int d = 0; d <= 9; ++d) {
        seq_t val = 0;
        idx_t temp = n;
        idx_t p = pow10;
        while (p > 0) {
            int digit = (temp / p) % 10;
            temp %= p;
            p /= 10;
            if (digit != d) {
                val = val * 10 + (seq_t)digit;
            }
        }
        if (first || val < best) {
            best = val;
            first = false;
        }
    }
    return best;
}
