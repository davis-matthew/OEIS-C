/*
 * A352544  a(n) = n/2 if n is even, otherwise n + A004186(n), where A004186 =
 *          arrange digits in decreasing order.
 * https://oeis.org/A352544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n/2 if n is even, otherwise n + A004186(n) where
 *            A004186(n) is the number formed by arranging the digits of n in
 *            decreasing order
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t digits_descending(idx_t n)
{
    /* Count digits 0-9 */
    unsigned int freq[10] = {0};
    idx_t tmp = n;
    if (tmp == 0) {
        freq[0] = 1;
    } else {
        while (tmp > 0) {
            freq[tmp % 10]++;
            tmp /= 10;
        }
    }

    /* Reconstruct number in descending order */
    seq_t result = 0;
    for (int d = 9; d >= 0; --d) {
        for (unsigned int k = 0; k < freq[d]; ++k) {
            result = result * 10 + (seq_t)d;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    if ((n & 1) == 0)          /* n even */
        return (seq_t)(n / 2);
    else                       /* n odd */
        return (seq_t)n + digits_descending(n);
}
