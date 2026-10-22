/*
 * A349731  a(n) = -(-n)^n * FallingFactorial(1/n, n) for n >= 1 and a(0) =
 *          -1.
 * https://oeis.org/A349731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -(-n)^n * FallingFactorial(1/n, n)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 7, i64 n <= 11, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t j = 0; j < n; ++j) {
        seq_t term = 1 - (seq_t)n * (seq_t)j;
        prod *= term;
    }
    /* The sign is (-1)^(n+1);   even n  -> sign −1, odd n -> +1   */
    if ((n & 1) == 0) {
        prod = -prod;
    }
    return prod;
}
