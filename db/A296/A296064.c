/*
 * A296064  a(1) = 0; thereafter a(n) is the smallest number (in absolute
 *          value) not yet in the sequence such that the arithmetic mean of
 *          the first n terms a(1), a(2), ..., a(n) is an integer. Preference
 *          is given to positive values of a(n).
 * https://oeis.org/A296064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1)=0, a(2)=2, a(3)=1; For n≥4, a(n) = (n%2==0) ? (1-n) : n
 * Range:     i8 n <= 128, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2) return 2;
    if (n == 3) return 1;
    return ((n & 1) == 0) ? ((seq_t)1 - (seq_t)n) : (seq_t)n;
}
