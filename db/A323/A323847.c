/*
 * A323847  a(n) = (n-1)*(n-2)*(n^2+9*n+12)/24.
 * https://oeis.org/A323847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n-2)*(n^2+9*n+12)/24
 * Range:     u8 n <= 7, u16 n <= 34, u32 n <= 416, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;       /* a(0) = 1 */
    if (n < 3) return 0;        /* a(1) = a(2) = 0 */

    seq_t N = n;
    seq_t part1 = (N - 1) * (N - 2);                      /* (n-1)*(n-2) */
    seq_t part2 = N * N + (seq_t)9 * N + (seq_t)12;       /* n^2+9n+12 */
    return part1 * part2 / (seq_t)24;                     /* divide by 24 */
}
