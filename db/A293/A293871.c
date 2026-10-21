/*
 * A293871  Numbers having 11 as substring of their digits.
 * https://oeis.org/A293871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 2771, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains_11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th positive integer whose decimal representation contains the substring "11" */
#include <stdbool.h>
#include "../utils/contains_11.h"

/* Return the n-th integer that contains the substring "11".  n >= 1. */
seq_t a(idx_t n) {
    seq_t cur = 11;          /* The first candidate that can contain "11". */
    idx_t count = 0;         /* How many qualifying numbers have been seen. */

    while (1) {
        if (contains_11(cur)) {
            ++count;
            if (count == n) return cur;
        }
        ++cur;               /* Check the next integer. */
    }
}
