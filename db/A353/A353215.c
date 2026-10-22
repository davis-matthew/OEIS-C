/*
 * A353215  a(n) is the result of n applications of the function f on n, where
 *          f(x) = floor((3*x - 1)/2) (A001651).
 * https://oeis.org/A353215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f^n(n) where f(x) = floor((3*x - 1)/2)
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    for (idx_t i = 0; i < n; ++i)
    {
        x = (3 * x - 1) / 2;
    }
    return x;
}
