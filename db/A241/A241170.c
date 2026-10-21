/*
 * A241170  Steffensen's bracket function [n,n-3].
 * https://oeis.org/A241170
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n-1)*(24 + 7*n + 3*n^2)/24
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 345, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is undefined – return 0 (any value is fine) */
    if (n < (idx_t)3) return 0;

    seq_t N = (seq_t)n;
    seq_t part1 = N - (seq_t)2;          /* (n-2) */
    seq_t part2 = N - (seq_t)1;          /* (n-1) */
    seq_t part3 = (seq_t)24 + (seq_t)7 * N + (seq_t)3 * N * N; /* 24 + 7*n + 3*n^2 */

    seq_t prod = part1 * part2;          /* (n-2)*(n-1) */
    prod = prod * part3;                 /* times the cubic part */
    return prod / (seq_t)24;             /* divide by 24 */
}
