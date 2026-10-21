/*
 * A267783  Number of n X 3 0..1 arrays with every repeated value in every row
 *          greater than or equal to, and in every column greater than, the
 *          previous repeated value.
 * https://oeis.org/A267783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+2)^2 / 4)^3
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 78, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t m   = N + 2;
    seq_t base = (m * m) / 4;
    seq_t ans   = base * base * base;
    return ans;
}
