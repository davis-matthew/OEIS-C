/*
 * A270695  Alternating sum of centered octagonal pyramidal numbers.
 * https://oeis.org/A270695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = ((-1)^n*(4*n^2 - 1)*(2*n + 3) + 3)/12
 * Range:     i8 n <= 5, i16 n <= 36, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    long long m = (long long)n;
    long long term1 = 4 * m * m - 1;
    long long term2 = 2 * m + 3;
    long long prod = term1 * term2;
    if (n & 1) {
        prod = -prod;
    }
    long long num = prod + 3;
    long long res = num / 12;
    return (seq_t)res;
}
