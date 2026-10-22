/*
 * A303692  a(n) = n^2*(2*n - 3 - (-1)^n)/4.
 * https://oeis.org/A303692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * floor((n-1)/2)
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    idx_t half_idx = (n - 1) / 2;
    seq_t half = (seq_t)half_idx;
    seq_t result = sn * sn;
    result *= half;
    return result;
}
