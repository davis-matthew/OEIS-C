/*
 * A295018  Squares whose largest digit is 8.
 * https://oeis.org/A295018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = square of the nth integer x whose square’s maximum
 *            decimal digit equals 8
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t x = 1; ; ++x) {
        seq_t sq = (seq_t)x * (seq_t)x;          /* compute square */
        seq_t temp = sq;
        seq_t maxd = 0;
        while (temp > 0) {                       /* find maximal digit of sq */
            seq_t d = temp % 10;
            if (d > maxd) maxd = d;
            temp /= 10;
        }
        if (maxd == 8) {                         /* report the n‑th found value */
            ++count;
            if (count == n) return sq;
        }
    }
}
