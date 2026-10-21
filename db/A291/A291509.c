/*
 * A291509  The arithmetic function uhat(n,2,4).
 * https://oeis.org/A291509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -n for n ≥ 9; for 1 ≤ n < 9, a(n) = -7 if n is odd, -8 if
 *            n is even
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is unsigned; use modulo 2 for parity */
    if (n < 9ULL) {
        return (n & 1ULL) ? (-(seq_t)7) : (-(seq_t)8);
    } else {
        return -(seq_t)n;
    }
}
