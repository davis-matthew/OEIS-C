/*
 * A268266  Number of length-(7+1) 0..n arrays with new repeated values
 *          introduced in sequential order starting with zero.
 * https://oeis.org/A268266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^8 + n^7 + 7*n^6 + 6*n^5 + 30*n^4 + 20*n^3 + 50*n^2 +
 *            20*n + 15 for n > 2, with a(1) = 129, a(2) = 1918
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)129;
    if (n == 2) return (seq_t)1918;

    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 r = N * N * N * N * N * N * N * N;      /* n^8 */
    r += N * N * N * N * N * N * N;                          /* n^7 */
    r += (unsigned __int128)7 * N * N * N * N * N * N;        /* 7 n^6 */
    r += (unsigned __int128)6 * N * N * N * N * N;            /* 6 n^5 */
    r += (unsigned __int128)30 * N * N * N * N;               /* 30 n^4 */
    r += (unsigned __int128)20 * N * N * N;                   /* 20 n^3 */
    r += (unsigned __int128)50 * N * N;                      /* 50 n^2 */
    r += (unsigned __int128)20 * N;                          /* 20 n   */
    r += (unsigned __int128)15;                              /* +15    */
    return (seq_t)r;
}
