/*
 * A302695  Number of 6-cycles in the (n+5)-path complement graph.
 * https://oeis.org/A302695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(4 + 22*n + 17*n^2 + 13*n^3 + 3*n^4 + n^5)/12
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit intermediate to avoid overflow on 64‑bit builds */
    __int128 N  = n;
    __int128 P  = 4
                + 22 * N
                + 17 * N * N
                + 13 * N * N * N
                +   3 * N * N * N * N
                +   N * N * N * N * N;
    __int128 num = N * P;
    __int128 res = num / 12;   /* exact division */
    return (seq_t)res;
}
