/*
 * A398157  a(n) = n/3 if n == 0 (mod 3), 4n+1 if n == 1 (mod 3), 2n-1 if n ==
 *          2 (mod 3).
 * https://oeis.org/A398157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/3 if n % 3 == 0, 4*n + 1 if n % 3 == 1, 2*n - 1 if n %
 *            3 == 2
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    if (r == 0) {
        return (seq_t)n / 3;
    } else if (r == 1) {
        return 4 * (seq_t)n + 1;
    } else { /* r == 2 */
        return 2 * (seq_t)n - 1;
    }
}
