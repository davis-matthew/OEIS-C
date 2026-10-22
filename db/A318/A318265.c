/*
 * A318265  a(1) = 1, a(n) = 5*a(n/3) if n is divisible by 3, otherwise a(n) =
 *          n - a(n-1).
 * https://oeis.org/A318265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(1) = 1, a(n) = 5*a(n/3) if n % 3 == 0, a(n) = n - a(n-1)
 *            otherwise
 * Range:     i8 n <= 32, i16 n <= 2186, i32 n <= 8914, i64 n <= 8914,
 *            i128 n <= 8914, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1u)
        return (seq_t)1;
    if ((n % 3u) == 0u)
        return (seq_t)5 * a(n / 3u);
    return (seq_t)n - a(n - 1u);
}
