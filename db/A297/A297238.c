/*
 * A297238  Up-variation of the base-13 digits of n; see Comments.
 * https://oeis.org/A297238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of d(i-1)−d(i) over all adjacent base‑13 digits where
 *            d(i) < d(i-1) (up‑variation of the base‑13 digits of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>   /* for uint8_t */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; safety check */
        return 0;

    /* n is 1‑based, convert to base‑13 digits (least significant first) */
    uint8_t digits[20];          /* 13^20 > 2^64, enough for any 64‑bit value */
    size_t len = 0;
    idx_t t = n;
    while (t > 0) {
        digits[len++] = (uint8_t)(t % 13);
        t /= 13;
    }
    if (len == 0) {              /* should not happen for n>=1 */
        digits[len++] = 0;
    }

    /* Sum the decreases between consecutive digits */
    seq_t sum = 0;
    for (size_t i = 1; i < len; ++i) {
        if (digits[i] < digits[i - 1]) {
            sum += (seq_t)(digits[i - 1] - digits[i]);
        }
    }
    return sum;
}
