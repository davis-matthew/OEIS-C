/*
 * A347272  Main diagonal of the square array A347270.
 * https://oeis.org/A347272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th iterate of the Collatz map applied to n+1 (i.e.
 *            a(n) = f^n(n+1) where f(x) = x/2 if x even, else 3x+1)
 * Range:     u8 n <= 25, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)(n + 1);
    for (idx_t i = 0; i < n; ++i)
    {
        if ((x & 1) == 0)
            x = x >> 1;
        else
            x = x * 3 + 1;
    }
    return x;
}
