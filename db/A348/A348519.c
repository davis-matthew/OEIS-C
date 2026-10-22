/*
 * A348519  Tetraphobe or 4-phobe numbers: integers that are not tetraphile
 *          numbers.
 * https://oeis.org/A348519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   is_tetraphile
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A348519: 4-phobe numbers (elements not representable as a sum of four
   increasing positive integers b1<b2<b3<b4 with b1|b2, b2|b3, b3|b4).
   The set is finite; it contains exactly 23 numbers, the largest being 48.
   This implementation enumerates all possible chains up to the known
   maximum and builds the sequence lazily.  The search is inexpensive
   because the largest tetraphobe is 48. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_tetraphile.h"

/* Main sequence function.  Builds the first 23 terms on the first call.
   Returns 0 for indices beyond the known finite set. */
seq_t a(idx_t n)
{
    /* The total number of tetraphobe numbers is known to be 23. */
    enum { COUNT = 23 };
    static bool built = false;
    static seq_t seq[COUNT];

    if (!built) {
        idx_t found = 0;
        idx_t candidate = 1;
        while (found < COUNT) {
            /* A tetraphobe is a number that is NOT tetraphile */
            if (!is_tetraphile((seq_t)candidate)) {
                seq[found++] = (seq_t)candidate;
            }
            ++candidate;
        }
        built = true;
    }

    if (n >= 1 && n <= COUNT) {
        return seq[n - 1];
    }
    /* For n beyond the finite list, the sequence is undefined. */
    return (seq_t)0;
}
