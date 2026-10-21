/*
 * A246481  Number of length 2+3 0..n arrays with no pair in any consecutive
 *          four terms totalling exactly n.
 * https://oeis.org/A246481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n even: n^5 - 4*n^4 + 10*n^3 - 11*n^2 + 5*n if n odd:
 *            n^5 - 4*n^4 + 10*n^3 - 4*n^2 -10*n + 9 (OEIS A246481)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 85, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t once */
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t N4 = N3 * N;
    seq_t N5 = N4 * N;

    seq_t res;
    if ((n & 1ULL) == 0)   /* even n */
    {
        res = N5 - (seq_t)4 * N4 + (seq_t)10 * N3 - (seq_t)11 * N2 + (seq_t)5 * N;
    }
    else                   /* odd n */
    {
        res = N5 - (seq_t)4 * N4 + (seq_t)10 * N3 - (seq_t)4 * N2 - (seq_t)10 * N + (seq_t)9;
    }
    return res;
}
