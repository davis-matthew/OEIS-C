/*
 * A317180  a(n) is the least positive multiple of n that contains at least
 *          one digit 1 in its decimal representation.
 * https://oeis.org/A317180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the least positive multiple of n that contains at least one digit 1
   by brute‑force search.  For each n >= 1 we test successive multiples
   of n until the decimal representation has a '1'. */
#include <stdbool.h>

/* Helper: does x contain the digit 1? */
static inline bool has_digit_one(seq_t x) {
    seq_t const one = (seq_t)1;
    if (x == 0) return false;           /* zero never needs the digit 1 */
    while (x != 0) {
        if (x % (seq_t)10 == one) return true;
        x /= (seq_t)10;
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n) {
    seq_t result;
    seq_t N = (seq_t)n;   /* cast index to seq_t once */
    seq_t mul = (seq_t)1;

    /* Iterate multiples of N until one contains the digit 1 */
    for (;;) {
        result = N * mul;
        if (has_digit_one(result)) break;
        ++mul;
    }
    return result;
}
