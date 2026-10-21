/*
 * A296747  Numbers whose base-12 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          = #(falls); see Comments.
 * https://oeis.org/A296747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: returns the nth positive integer whose base‑12 digits have an equal number of rises and falls */

#include <stdbool.h>

static inline bool equal_rises_falls(idx_t x) {
    int rises = 0, falls = 0;
    if (x == 0) return true;           /* 0 will never be queried */
    unsigned prev = x % 12u;            /* least significant digit */
    x /= 12u;
    while (x > 0) {
        unsigned curr = x % 12u;        /* next digit toward the most significant */
        if (prev < curr) rises++;
        else if (prev > curr) falls++;
        prev = curr;
        x /= 12u;
    }
    return rises == falls;
}

seq_t a(idx_t n) {
    idx_t found = 0;
    for (idx_t x = 1;; ++x) {
        if (equal_rises_falls(x)) {
            ++found;
            if (found == n) {
                return (seq_t)x;
            }
        }
    }
}
