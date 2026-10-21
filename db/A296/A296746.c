/*
 * A296746  Numbers whose base-11 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          < #(falls); see Comments.
 * https://oeis.org/A296746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th positive integer whose base‑11 digit string has
 *            fewer rises than falls (i.e., rise < fall). The sequence is
 *            defined by direct examination of each integer, so this
 *            implementation simply enumerates successive integers until the
 *            n‑th qualifying number is found
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   in_A296746
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/in_A296746.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cur = (seq_t)1;
    while (1) {
        if (in_A296746(cur)) {
            ++found;
            if (found == n) return cur;
        }
        ++cur;
    }
}
