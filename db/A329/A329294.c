/*
 * A329294  Numbers whose digits are in nondecreasing order in bases 4 and 5.
 * https://oeis.org/A329294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth nonnegative integer whose base‑4 and base‑5 digit
 *            strings are in nondecreasing order
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   nondecreasing_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/nondecreasing_digits.h"

/* Return the nth term of OEIS A329294. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1)     /* a(1) = 0 */
        return (seq_t)0;

    idx_t count = (idx_t)1;    /* count already includes 0 */
    seq_t x = (seq_t)1;

    while (count < n) {
        if (nondecreasing_digits(x, 4) && nondecreasing_digits(x, 5)) {
            ++count;
            if (count == n)
                return x;
        }
        x = x + (seq_t)1;
    }

    /* In theory we never reach here because the sequence is infinite. */
    return (seq_t)0;
}
