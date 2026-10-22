/*
 * A354158  The Bodlaender function: a(1) = -1; a(2*n) = a(n), a(2*n+1) =
 *          a(n+1) + n.
 * https://oeis.org/A354158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = -1; a(2*n) = a(n); a(2*n+1) = a(n+1) + n
 * Range:     i8 n <= 144, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)-1;
    }
    if ((n & 1) == 0) {
        /* even case: a(2*n) = a(n) */
        return a(n >> 1);
    } else {
        /* odd case: a(2*n+1) = a(n+1) + n, here n = (original n - 1)/2 */
        return a((n >> 1) + 1) + (seq_t)(n >> 1);
    }
}
