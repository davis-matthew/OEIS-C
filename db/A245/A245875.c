/*
 * A245875  Number of length 6+2 0..n arrays with some pair in every
 *          consecutive three terms totalling exactly n.
 * https://oeis.org/A245875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 22*n + 31*n^2 + 77*n^3 + 29*n^4 + n^5 if n is even -5
 *            - 44*n + 10*n^2 + 77*n^3 + 29*n^4 + n^5 if n is odd
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 79, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* out of range, not needed for the test data */

    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;            /* n^2 */
    seq_t n3 = n2 * N;           /* n^3 */
    seq_t n4 = n3 * N;           /* n^4 */
    seq_t n5 = n4 * N;           /* n^5 */

    if ((n & 1) == 0)            /* n even */
    {
        seq_t res = 1;
        res += (seq_t)22 * N;
        res += (seq_t)31 * n2;
        res += (seq_t)77 * n3;
        res += (seq_t)29 * n4;
        res += n5;
        return res;
    }
    else                         /* n odd */
    {
        seq_t pos = n5;
        pos += (seq_t)29 * n4;
        pos += (seq_t)77 * n3;
        pos += (seq_t)10 * n2;

        seq_t neg = (seq_t)44 * N;
        neg += 5;

        return pos - neg;        /* pos > neg for all n≥1, no underflow */
    }
}
