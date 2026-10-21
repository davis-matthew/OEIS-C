/*
 * A255642  a(n) = n*((n mod 3)*(n mod 5)-((n mod 3)+(n mod 5))).
 * https://oeis.org/A255642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*((n mod 3)*(n mod 5)-((n mod 3)+(n mod 5)))
 * Range:     i8 n <= 38, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;
    seq_t r3  = (seq_t)(n % 3);
    seq_t r5  = (seq_t)(n % 5);
    seq_t prod = r3 * r5 - (r3 + r5);
    return nn * prod;
}
