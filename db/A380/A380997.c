/*
 * A380997  a(n) is the least number with exactly 2 different decimal digits
 *          that is a multiple of n.
 * https://oeis.org/A380997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) is the least multiple of n that has exactly two distinct
 *            decimal digits
 * Range:     u8 n <= 32, u16 n <= 169, u32 n <= 809, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9539 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: return true if v has exactly two different decimal digits. */
static inline bool has_two_digits(seq_t v) {
    bool seen[10] = {0};
    int cnt = 0;
    while (v != 0) {
        int d = (int)(v % 10);
        if (!seen[d]) {
            seen[d] = true;
            if (++cnt > 2) return false;
        }
        v /= 10;
    }
    return cnt == 2;
}

/* Entry point: a(n) */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined but guard */
    for (idx_t k = 1;; ++k) {
        seq_t m = (seq_t)k * (seq_t)n;
        if (has_two_digits(m))
            return m;
    }
}
