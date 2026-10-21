/*
 * A247353  Number of paths from (0,1) to the line x = n, each consisting of
 *          segments given by the vectors (1,1), (1,2), (1,-1), with vertices
 *          (i,k) satisfying 0 <= k <= 3.
 * https://oeis.org/A247353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) + 2*a(n-3) - a(n-4) Initial values: a(0)=1,
 *            a(1)=3, a(2)=5, a(3)=11
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 66, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)11;

    /* Use a circular buffer of length 4 to keep the last four values. */
    seq_t buf[4] = {(seq_t)1, (seq_t)3, (seq_t)5, (seq_t)11};

    for (idx_t i = 4; i <= n; ++i)
    {
        /* Indices modulo 4:   i-2, i-3, i-4 */
        idx_t i2 = (i + 2) % 4;   /* i-2 */
        idx_t i3 = (i + 1) % 4;   /* i-3 */
        idx_t i4 = i % 4;         /* i-4 */
        buf[i % 4] = (seq_t)3 * buf[i2] + (seq_t)2 * buf[i3] - buf[i4];
    }

    return buf[n % 4];
}
