/*
 * A270029  a(n) is the smallest b for which the base-b representation of n
 *          contains at least one 3 (or 0 if no such base exists).
 * https://oeis.org/A270029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base b >= 4 such that the base‑b representation
 *            of n * contains at least one digit 3; returns 0 if no such base
 *            exists
 * Range:     u8 n <= 536, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains_digit_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required types supplied by the build system: seq_t, idx_t */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains_digit_three.h"

/* Main function implementing the sequence. */
seq_t a(idx_t n)
{
    /* Handle trivial cases explicitly. */
    if (n == 1 || n == 2 || n == 4 || n == 5 || n == 6)
        return (seq_t)0;
    if (n == 3)
        return (seq_t)4;

    /* Search for the smallest base b >= 4 with digit 3. */
    for (idx_t b = 4; b <= n; ++b)
    {
        if (contains_digit_three(n, b))
            return (seq_t)b;
    }
    return (seq_t)0;
}
