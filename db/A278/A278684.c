/*
 * A278684  Number of non-equivalent ways to place 5 non-attacking ferses on
 *          an n X n board.
 * https://oeis.org/A278684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 18, u64 n <= 167, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (n^10 - 50*n^8 + 80*n^7 + 955*n^6 - 2828*n^5 - 7090*n^4
 *                + 36860*n^3 - 10856*n^2 - 133712*n + 161280
 *                + ((1-(-1)^n)/2)*(52*n^5 - 145*n^4 - 580*n^3 + 2320*n^2
 *                - 1152*n - 15)) / 960   for n >= 4
 *
 * a(1) = 0, a(2) = 0, a(3) = 2
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the first three terms explicitly */
    if (n == 1) return 0;
    if (n == 2) return 0;
    if (n == 3) return 2;

    /* use unsigned __int128 for intermediate arithmetic */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n2 = nn * nn;
    unsigned __int128 n3 = n2 * nn;
    unsigned __int128 n4 = n3 * nn;
    unsigned __int128 n5 = n4 * nn;
    unsigned __int128 n6 = n5 * nn;
    unsigned __int128 n7 = n6 * nn;
    unsigned __int128 n8 = n7 * nn;
    unsigned __int128 n10 = n8 * n2;

    unsigned __int128 num =
        n10
        - (unsigned __int128)50 * n8
        + (unsigned __int128)80 * n7
        + (unsigned __int128)955 * n6
        - (unsigned __int128)2828 * n5
        - (unsigned __int128)7090 * n4
        + (unsigned __int128)36860 * n3
        - (unsigned __int128)10856 * n2
        - (unsigned __int128)133712 * nn
        + (unsigned __int128)161280;

    /* add the parity-dependent part if n is odd */
    if (n & 1) {
        num += (unsigned __int128)52 * n5
             - (unsigned __int128)145 * n4
             - (unsigned __int128)580 * n3
             + (unsigned __int128)2320 * n2
             - (unsigned __int128)1152 * nn
             - (unsigned __int128)15;
    }

    unsigned __int128 res = num / (unsigned __int128)960;
    return (seq_t)res;
}
