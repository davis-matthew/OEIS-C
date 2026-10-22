/*
 * A372987  a(n) = (2*n)!/(n!*a(n-1)), with a(0)=1.
 * https://oeis.org/A372987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..floor((n-1)/2)} 2*(2*n-4*k-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 24, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t m = (n - 1) / 2;

    for (idx_t k = 0; k <= m; ++k)
    {
        seq_t term = (seq_t)2 *
                     (((seq_t)2 * n) - (seq_t)4 * k - (seq_t)1);
        result *= term;
    }

    return result;
}
