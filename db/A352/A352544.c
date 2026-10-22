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
 * Helpers:   digits_descending
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/digits_descending.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0)          /* n even */
        return (seq_t)(n / 2);
    else                       /* n odd */
        return (seq_t)n + digits_descending(n);
}
