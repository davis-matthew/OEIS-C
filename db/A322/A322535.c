/*
 * A322535  Number of arrays of 8 integers in -n..n with sum zero.
 * https://oeis.org/A322535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            (2*n+1)*(9664*n^6+28992*n^5+37360*n^4+26400*n^3+10936*n^2+2568*n+315)/315
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 12, u64 n <= 313,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic to avoid overflow of intermediate results */
    unsigned __int128 N = n;           /* cast idx_t to a larger type */
    unsigned __int128 n2 = N * N;      /* n^2 */
    unsigned __int128 n3 = n2 * N;     /* n^3 */
    unsigned __int128 n4 = n2 * n2;    /* n^4 */
    unsigned __int128 n5 = n4 * N;     /* n^5 */
    unsigned __int128 n6 = n3 * n3;    /* n^6 */

    unsigned __int128 p =
        (unsigned __int128)9664 * n6 +
        (unsigned __int128)28992 * n5 +
        (unsigned __int128)37360 * n4 +
        (unsigned __int128)26400 * n3 +
        (unsigned __int128)10936 * n2 +
        (unsigned __int128)2568 * N +
        315;

    unsigned __int128 prod = ( (unsigned __int128)2 * N + 1 ) * p;
    return (seq_t)(prod / 315);
}
