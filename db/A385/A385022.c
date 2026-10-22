/*
 * A385022  Table read by rows: row n is the unique primitive Pythagorean
 *          triple (a,b,c) such that (a-b+c)/2 = A002378(n) and its long leg
 *          and hypotenuse are consecutive natural numbers.
 * https://oeis.org/A385022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabf
 * Formula:   let r = ⌊(n-1)/3⌋ + 1, t = r*(r+1). Then a(n) = 2*t-1 if (n-1)
 *            mod 3 = 0, 2*t*(t-1) if (n-1) mod 3 = 1, 2*t*(t-1)+1 otherwise
 * Range:     i8 n <= 7, i16 n <= 31, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Ensure n >= 1; behaviour for n == 0 is undefined. */
    idx_t r = (n - 1) / 3 + 1;           /* row number, 1‑based */
    seq_t rr = (seq_t)r;
    seq_t t = rr * (rr + 1);             /* t = r*(r+1) */
    idx_t pos = (n - 1) % 3;             /* position in the triple */

    if (pos == 0) {
        return 2 * t - 1;                /* a */
    } else if (pos == 1) {
        return 2 * t * (t - 1);          /* b */
    } else {
        return 2 * t * (t - 1) + 1;      /* c */
    }
}
