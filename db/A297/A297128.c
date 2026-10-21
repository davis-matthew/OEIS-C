/*
 * A297128  Numbers having an up-first zigzag pattern in base 4; see Comments.
 * https://oeis.org/A297128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   up_first_zigzag
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers having an up‑first zigzag pattern in base 4
 * (OEIS A297128).
 * No closed‑form formula exists; the function searches the positive
 * integers in increasing order and returns the n‑th qualifying number.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/up_first_zigzag.h"

seq_t a(idx_t n)
{
    idx_t count = 0;  /* how many qualifying numbers have been seen */
    seq_t val = 1;    /* candidate value, start at 1 */

    while (1) {
        if (up_first_zigzag(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++val;  /* move to the next positive integer */
    }
}
