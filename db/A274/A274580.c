/*
 * A274580  Digital difference of n: the most significant decimal digit of n
 *          minus the sum of the other digits.
 * https://oeis.org/A274580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, base
 * Formula:   a(n) = most significant decimal digit of n minus the sum of the
 *            other decimal digits
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the digital difference of n:
   Let d1 be the most significant digit of n and d2…dk the other digits.
   Return a(n) = d1 − (d2+…+dk). */
seq_t a(idx_t n)
{
    /* Sum all digits of n. */
    idx_t x = n;
    seq_t sum = 0;
    while (x > 0) {
        sum += (seq_t)(x % 10);
        x /= 10;
    }

    /* Find the most significant digit. */
    idx_t y = n;
    while (y >= 10) {
        y /= 10;
    }
    seq_t msd = (seq_t)y;

    /* Compute the result: msd - (sum - msd) = 2*msd - sum. */
    return msd * 2 - sum;
}
