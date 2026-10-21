/*
 * A270259  Number of 4X4X4 triangular 0..n arrays with some element plus some
 *          adjacent element totalling n exactly once.
 * https://oeis.org/A270259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0 * a(n) = 18*n^9 - 144*n^8 + 612*n^7 - 1746*n^6 +
 *            3618*n^5 - 5502*n^4 * + 5904*n^3 - 4002*n^2 + 1278*n , n even *
 *            * a(n) = 18*n^9 - 144*n^8 + 585*n^7 - 1440*n^6 + 2178*n^5 -
 *            1641*n^4 * - 522*n^3 + 2502*n^2 - 2259*n + 723 , n odd (n>1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0; a(0) undefined but return 0 */

    __int128 n128 = ( __int128 )n;
    __int128 n2   = n128 * n128;
    __int128 n3   = n2 * n128;
    __int128 n4   = n3 * n128;
    __int128 n5   = n4 * n128;
    __int128 n6   = n5 * n128;
    __int128 n7   = n6 * n128;
    __int128 n8   = n7 * n128;
    __int128 n9   = n8 * n128;

    __int128 res;
    if ( (n & 1ULL) == 0 ) {               /* even n */
        res =  (__int128)18 * n9
             - (__int128)144 * n8
             + (__int128)612 * n7
             - (__int128)1746 * n6
             + (__int128)3618 * n5
             - (__int128)5502 * n4
             + (__int128)5904 * n3
             - (__int128)4002 * n2
             + (__int128)1278 * n128;
    } else {                               /* odd n > 1 */
        res =  (__int128)18 * n9
             - (__int128)144 * n8
             + (__int128)585 * n7
             - (__int128)1440 * n6
             + (__int128)2178 * n5
             - (__int128)1641 * n4
             - (__int128)522 * n3
             + (__int128)2502 * n2
             - (__int128)2259 * n128
             + (__int128)723;
    }

    return (seq_t)res;   /* cast to the result type; safe for 64‑bit and big builds */
}
