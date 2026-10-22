/*
 * A375213  Even numbers with equal numbers of even and odd digits.
 * https://oeis.org/A375213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_equal_even_odd_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search over even numbers, checking digit parity. */
#include <stdbool.h>
#include "../utils/has_equal_even_odd_digits.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t num = 10;               /* first two‑digit even number */

    while (1) {
        if (has_equal_even_odd_digits(num)) {
            ++count;
            if (count == n)
                return num;
        }
        num += 2;                 /* only even candidates */
    }
}
