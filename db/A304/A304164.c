/*
 * A304164  a(n) = 27*n^2 - 21*n + 6 (n>=1).
 * https://oeis.org/A304164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*n^2 - 21*n + 6
 * Range:     u8 n <= 3, u16 n <= 49, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; return 0 for n = 0 (unused) */
        return 0;
    seq_t N = (seq_t)n;
    return 27 * N * N - 21 * N + 6;
}
