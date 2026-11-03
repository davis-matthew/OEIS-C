/*
 * A293871  Numbers having 11 as substring of their digits.
 * https://oeis.org/A293871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 2771, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9597 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th positive integer whose decimal representation contains the substring "11" */
#include <stdbool.h>

/* Return true if x's decimal digits contain two consecutive 1's. */
static bool contains_11(seq_t x) {
    /* Numbers with fewer than two digits cannot contain "11". */
    if (x < 11) return false;

    seq_t last = x % 10;
    x /= 10;
    while (x > 0) {
        seq_t d = x % 10;
        if (d == 1 && last == 1) return true;
        last = d;
        x /= 10;
    }
    return false;
}

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
