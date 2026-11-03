/*
 * A241157  Numbers in which the two least-significant digits are distinct.
 * https://oeis.org/A241157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th nonnegative integer whose two least‑significant
 *            decimal digits are distinct. For n < 10 the condition is
 *            automatically satisfied
 * Range:     u8 n <= 231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9802 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool distinct_last_two_digits(seq_t x) {
    if (x < (seq_t)10)          /* 0 … 9 are all accepted */
        return true;
    seq_t last = x % (seq_t)10;
    seq_t tens = (x / (seq_t)10) % (seq_t)10;
    return last != tens;
}

seq_t a(idx_t n) {
    /* The OEIS offset is 1, so n ≥ 1.  Convert once to seq_t for arithmetic. */
    seq_t target = (seq_t)n;
    seq_t count = 0;
    seq_t cur   = 0;

    while (count < target) {
        if (distinct_last_two_digits(cur))
            count++;
        cur++;   /* advance to next integer */
    }

    /* cur has been incremented past the desired value */
    return cur - 1;
}
