/*
 * A354670  Number of graph minors in the n-book graph.
 * https://oeis.org/A354670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (60480 + 19152*n + 130148*n^2 + 13356*n^3 + 9947*n^4 +
 *            7728*n^5 + 1022*n^6 + 84*n^7 + 3*n^8) / 20160, for n > 1 a(1) =
 *            13
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 13;

    /* Use unsigned __int128 for safe intermediate calculations */
    unsigned __int128 N  = n;
    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n3 * N;
    unsigned __int128 n5 = n4 * N;
    unsigned __int128 n6 = n5 * N;
    unsigned __int128 n7 = n6 * N;
    unsigned __int128 n8 = n7 * N;

    unsigned __int128 numer = (unsigned __int128)60480
                            + (unsigned __int128)19152 * N
                            + (unsigned __int128)130148 * n2
                            + (unsigned __int128)13356 * n3
                            + (unsigned __int128)9947  * n4
                            + (unsigned __int128)7728  * n5
                            + (unsigned __int128)1022  * n6
                            + (unsigned __int128)84    * n7
                            + (unsigned __int128)3     * n8;

    return (seq_t)(numer / (unsigned __int128)20160);
}
