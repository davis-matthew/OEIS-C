/*
 * A337004  Turn sequence of the R5 dragon curve.
 * https://oeis.org/A337004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = 1 if the lowest non‑zero digit of n in base 5 is 1 or 2,
 *            else a(n) = -1
 * Range:     i8 n <= 3125, i16 n <= 3125, i32 n <= 3125, i64 n <= 3125,
 *            i128 n <= 3125, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* n is assumed ≥ 1 (offset 1) */
    idx_t t = n;
    while ((t % 5) == 0) {
        t /= 5;
    }
    seq_t digit = (seq_t)(t % 5);
    return (digit == 1 || digit == 2) ? (seq_t)1 : (seq_t)-1;
}
