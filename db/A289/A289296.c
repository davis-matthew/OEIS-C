/*
 * A289296  a(n) = (n - 1)*(2*floor(n/2) + 1).
 * https://oeis.org/A289296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n - 1)*(2*floor(n/2) + 1)
 * Range:     i8 n <= 11, i16 n <= 181, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    /* even n: n^2 - 1, odd n: n^2 - n */
    if ((n & 1) == 0)
        return n_seq * n_seq - 1;
    else
        return n_seq * n_seq - n_seq;
}
