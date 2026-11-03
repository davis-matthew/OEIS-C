/*
 * A370918  Positive integers whose ternary representation includes at least
 *          one 1, and every 1 is followed by 0.
 * https://oeis.org/A370918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose ternary representation
 *            contains at least one digit 1 and every 1 is immediately
 *            followed by a 0 (i.e. no 1 is allowed unless the next lower
 *            digit is 0). The function enumerates integers, tests the
 *            property, and returns the n‑th valid one
 * Range:     u8 n <= 45, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool valid_ternary(seq_t v) {
    unsigned char prev = 3;          /* sentinel: “no previous digit yet”   */
    bool found_one = false;

    while (v != 0) {
        unsigned char d = (unsigned char)(v % (seq_t)3);

        if (d == 1) {
            if (prev != 0)            /* the preceding digit must be 0 */
                return false;
            found_one = true;
        }
        prev = d;
        v /= (seq_t)3;
    }
    return found_one;                /* must contain at least one 1 */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* offset is 1; behaviour for n<1 is undefined */
    idx_t count   = 0;
    seq_t x       = 1;

    while (1) {
        if (valid_ternary(x)) {
            ++count;
            if (count == n) return x;
        }
        ++x;
    }
}
