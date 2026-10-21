/*
 * A268461  Number of length-7 0..n arrays with no adjacent pair x,x+1
 *          followed at any distance by x+1,x.
 * https://oeis.org/A268461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 29 (for n=1), a(n) = n^7 + 7*n^6 + 16*n^5 + 5*n^4 - 7*n^3
 *            + 18*n^2 - 5*n - 5 (for n>1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset: index 1 => first term */
    if (n == 1)
        return (seq_t)29;

    /* Compute with intermediate 128‑bit arithmetic to avoid overflow */
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 p2 = N * N;        /* n^2  */
    unsigned __int128 p3 = p2 * N;       /* n^3  */
    unsigned __int128 p4 = p3 * N;       /* n^4  */
    unsigned __int128 p5 = p4 * N;       /* n^5  */
    unsigned __int128 p6 = p5 * N;       /* n^6  */
    unsigned __int128 p7 = p6 * N;       /* n^7  */

    unsigned __int128 res =
        p7
        + (unsigned __int128)7 * p6
        + (unsigned __int128)16 * p5
        + (unsigned __int128)5 * p4
        - (unsigned __int128)7 * p3
        + (unsigned __int128)18 * p2
        - (unsigned __int128)5 * N
        - (unsigned __int128)5;

    return (seq_t)res;
}
