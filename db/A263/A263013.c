/*
 * A263013  a(0) = -a(1) = a(2) = 1, a(n) = 0 for n>2.
 * https://oeis.org/A263013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = -1, a(2) = 1, a(n) = 0 for n > 2 */
seq_t a(idx_t n)
{
    switch (n) {
        case 0: return 1;
        case 1: return (seq_t)(-1);
        case 2: return 1;
        default: return 0;
    }
}
