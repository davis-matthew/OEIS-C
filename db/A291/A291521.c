/*
 * A291521  The arithmetic function uhat(n,4,6).
 * https://oeis.org/A291521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -n for n >= 11; for n <= 10, a(n) = -11 if n odd, -12 if
 *            n even
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle n < 1 gracefully (offset is 1) */
    if (n == 0) {
        return (seq_t)0;
    }

    if (n <= 10) {
        /* n is odd -> -11, even -> -12 */
        if (n & 1) {
            return (seq_t)-11;
        } else {
            return (seq_t)-12;
        }
    } else {
        /* n >= 11: simply return -n */
        return -( (seq_t)n );
    }
}
