/*
 * A333970  Irregular triangle read by rows where the n-th row lists the bases
 *          2<=b<=n+1 where n in base b contains the digit b-1.
 * https://oeis.org/A333970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf, uned
 * Formula:   a(n) concatenates, row by row, the bases b (2 ≤ b ≤ row+1) for
 *            which n written in base b contains the digit b‑1. The sequence
 *            is flattened row-wise
 * Range:     u8 n <= 1896, u16 n <= 10027, u32 n <= 10027, u64 n <= 10027,
 *            u128 n <= 10027, big any n
 * Validated: standard, pass, 102 terms through n = 9684 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether a number n (row index) written in base b contains digit b-1.*/
static bool contains_digit_b_minus_1(idx_t n, idx_t b)
{
    while (n) {
        if ((n % b) == (b - 1u))
            return true;
        n /= b;
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t pos = 1;          /* current position in the flattened sequence */
    for (idx_t row = 1;; ++row) {
        for (idx_t b = 2; b <= row + 1; ++b) {
            if (contains_digit_b_minus_1(row, b)) {
                if (pos == n)
                    return (seq_t)b;
                ++pos;
            }
        }
    }
}
