/*
 * A284066  Numbers whose smallest decimal digit is 5.
 * https://oeis.org/A284066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   contains_min_five
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Find the n-th positive integer whose decimal
   representation contains only digits 5–9 and at least one digit 5.
   The sequence is A284066. */
#include <stdbool.h>
#include "../utils/contains_min_five.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Offset starts at 1; behavior for n<1 is undefined */

    idx_t count = 0;   /* number of terms found so far */
    idx_t num    = 0;   /* current candidate integer */

    while (1) {
        if (contains_min_five(num)) {
            ++count;
            if (count == n)
                return (seq_t)num;
        }
        ++num;
    }
}
