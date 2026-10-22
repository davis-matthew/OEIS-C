/*
 * A317336  a(n) = A317333(n) - 8*n.
 * https://oeis.org/A317336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = -7 For n > 1: if n is even: a(n) = -7 if n % 4 == 0 a(n)
 *            = -10 if n % 4 == 2 if n is odd: a(n) = a((n + 1) / 2)
 * Range:     i8 n <= 9155, i16 n <= 9155, i32 n <= 9155, i64 n <= 9155,
 *            i128 n <= 9155, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)-7;

    for (;;) {
        if ((n & 1) == 0) {          /* n is even */
            return ((n & 3) == 0) ? (seq_t)-7 : (seq_t)-10;
        } else {                     /* n is odd (n > 1) */
            n = (n >> 1) + 1;        /* (n + 1) / 2, no overflow risk */
        }
    }
}
