/*
 * A241787  Triangular numbers which have one or more occurrences of exactly
 *          four different digits.
 * https://oeis.org/A241787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth triangular number whose decimal representation
 *            contains exactly four distinct digits
 * Range:     u8 none, u16 n <= 175, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 59 terms through n = 1910 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check whether a number has exactly four distinct decimal digits. */
static inline bool has_four_distinct_digits(seq_t x) {
    unsigned mask = 0;          /* 10-bit mask for digits 0–9 */
    while (x) {
        unsigned digit = (unsigned)(x % 10);
        mask |= 1u << digit;
        x /= 10;
    }
    return __builtin_popcount(mask) == 4;
}

/* Return the nth term of A241787 (1‑based index). */
seq_t a(idx_t n) {
    idx_t found = 0;
    for (idx_t k = 1;; ++k) {
        seq_t t = (seq_t)k * ((seq_t)k + 1) / 2;   /* triangular number */
        if (has_four_distinct_digits(t)) {
            ++found;
            if (found == n)
                return t;
        }
    }
}
