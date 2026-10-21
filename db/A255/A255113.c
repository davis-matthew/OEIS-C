/*
 * A255113  Number of length n+6 0..2 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * (n^5 + 45*n^4 + 745*n^3 + 37815*n^2 + 112834*n + 46200)
 *            / 120 for n>4 * a(1) = 2187, a(2) = 5157, a(3) = 7498, a(4) =
 *            10125
 * Range:     u8 none, u16 n <= 11, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small n explicitly */
    if (n == 1) return (seq_t)2187;
    if (n == 2) return (seq_t)5157;
    if (n == 3) return (seq_t)7498;
    if (n == 4) return (seq_t)10125;
    /* n >= 5 : use the polynomial formula */
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 pow2 = N * N;               /* n^2 */
    unsigned __int128 pow3 = pow2 * N;            /* n^3 */
    unsigned __int128 pow4 = pow3 * N;            /* n^4 */
    unsigned __int128 pow5 = pow4 * N;            /* n^5 */

    unsigned __int128 num =   pow5
                            + (unsigned __int128)45 * pow4
                            + (unsigned __int128)745 * pow3
                            + (unsigned __int128)37815 * pow2
                            + (unsigned __int128)112834 * N
                            + (unsigned __int128)46200;

    return (seq_t)(num / 120);
}
