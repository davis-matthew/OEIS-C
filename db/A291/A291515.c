/*
 * A291515  The arithmetic function uhat(n,3,6).
 * https://oeis.org/A291515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 64, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula:
   a(n) = -2*n  for n >= 12;
   a(1) = -17, a(2) = -17, a(3) = -19, a(4) = -17, a(5) = -17,
   a(6) = -19, a(7) = -17, a(8) = -17, a(9) = -19, a(10) = -20,
   a(11) = -22
*/
static const seq_t init_values[11] = {
    -17, -17, -19,
    -17, -17, -19,
    -17, -17, -19,
    -20, -22
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset, not used */
    if (n <= 11) return init_values[n - 1];
    /* Linear recurrence simplifies to a(n) = -2*n for n >= 12 */
    return -( (seq_t)2 * (seq_t)n );
}
