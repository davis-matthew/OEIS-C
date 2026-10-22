/*
 * A353220  a(n) is the result of n applications of the function f to n, where
 *          f(x) = floor((3*x + 1)/2) (A007494).
 * https://oeis.org/A353220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f^n(n) where f(x) = floor((3*x + 1)/2) (OEIS A353220)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* start from the index itself */
    for (idx_t i = 0; i < n; ++i) {
        x = (x * 3 + 1) / 2;     /* floor((3*x + 1)/2) using integer division */
    }
    return x;
}
