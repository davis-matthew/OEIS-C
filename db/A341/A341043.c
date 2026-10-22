/*
 * A341043  a(n) = 16*n^3 - 36*n^2 + 30*n - 9.
 * https://oeis.org/A341043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^3 - 36*n^2 + 30*n - 9
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit signed arithmetic to avoid overflow
       in intermediate calculations, then convert to seq_t. */
    __int128 N = (__int128)n;
    __int128 val = 16 * N * N * N - 36 * N * N + 30 * N - 9;
    return (seq_t)val;
}
