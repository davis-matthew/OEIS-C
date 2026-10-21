/*
 * A241746  Smallest number greater than n that CANNOT be scored using n darts
 *          on a standard dartboard.
 * https://oeis.org/A241746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 23; for n >= 2, a(n) = 60*n - 17
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)23;
    return (seq_t)(60 * n - (seq_t)17);
}
