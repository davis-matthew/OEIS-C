/*
 * A257055  a(n) = n*(n + 1)*(n^2 - n + 3)/6.
 * https://oeis.org/A257055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 1)*(n^2 - n + 3)/6
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 366, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t t1 = N + 1;              /* N + 1        */
    seq_t t2 = N * t1;             /* N*(N+1)      */
    seq_t t3 = N * N;              /* N^2          */
    seq_t t4 = t3 - N + 3;          /* N^2 - N + 3  */
    seq_t t5 = t2 * t4;             /* product      */
    return t5 / 6;                  /* divide by 6  */
}
