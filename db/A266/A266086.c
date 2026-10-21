/*
 * A266086  Alternating sum of 9-gonal (or nonagonal) numbers.
 * https://oeis.org/A266086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((14*n*n + 4*n - 5)*(-1)^n + 5)/8
 * Range:     i8 n <= 8, i16 n <= 136, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert index to signed integer for intermediate arithmetic. */
    long long nn = (long long)n;

    /* Compute numerator of the formula in a type that is large enough. */
    long long num = 14LL * nn * nn + 4LL * nn - 5LL;

    /* (-1)^n : +1 for even n, -1 for odd n. */
    long long sign = (n & 1) ? -1LL : 1LL;

    /* Multiply by sign, add the constant 5 and divide by 8. */
    long long val  = num * sign + 5LL;
    long long res  = val / 8LL;

    return (seq_t)res;
}
