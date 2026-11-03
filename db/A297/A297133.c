/*
 * A297133  Numbers whose base-5 digits d(m), d(m-1),..., d(0) have m=0 or
 *          else d(i) = d(i+1) for some i in {0,1,...,m-1}.
 * https://oeis.org/A297133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search for the nth integer whose base‑5 representation
   contains at least one pair of consecutive equal digits,
   or consists of a single digit (m = 0). */

#include <stdbool.h>

static bool has_adjacent(seq_t v)
{
    /* Single‑digit numbers are always accepted. */
    if (v < 5) return true;

    seq_t last = v % 5;
    v /= 5;
    while (v > 0) {
        seq_t cur = v % 5;
        if (cur == last) return true;
        last = cur;
        v /= 5;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1).  Count each qualifying integer
       until we reach the nth one. */
    seq_t count = 0;
    seq_t value = 0;
    while (true) {
        ++value;
        if (has_adjacent(value)) {
            ++count;
            if (count == n) return value;
        }
    }
}
