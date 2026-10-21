/*
 * A270031  a(n) is the smallest b for which the base-b representation of n
 *          contains at least one 5 (or 0 if no such base exists).
 * https://oeis.org/A270031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base b ≥ 6 such that the base‑b representation
 *            of n contains a digit 5; 0 if no such base exists (n = 5 gives
 *            6)
 * Range:     u8 n <= 921, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if the representation of 'value' in base 'base' contains a
   digit equal to 5. */
static bool has_digit5(seq_t value, seq_t base) {
    while (value > 0) {
        if (value % base == 5)
            return true;
        value /= base;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n < 5)
        return 0;
    if (n == 5)
        return 6;          /* base 6 gives digit 5 for n = 5 */

    seq_t N = (seq_t)n;
    for (idx_t b = 6; b <= n; ++b) {
        seq_t B = (seq_t)b;
        if (has_digit5(N, B))
            return B;
    }
    return 0;              /* only possible for n = 6…10 */
}
