/*
 * A341768  a(n) = n * (binomial(n,2) - 2).
 * https://oeis.org/A341768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^2*(n-1)/2 - 2*n
 * Range:     i8 n <= 6, i16 n <= 40, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    long long tn = (long long)n;
    long long temp = tn * (tn - 1) / 2;
    long long result = tn * (temp - 2);
    return (seq_t)result;
}
