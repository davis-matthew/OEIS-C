/*
 * A290364  Number of 10-leaf rooted trees with n levels.
 * https://oeis.org/A290364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7936*n^9 + 4635*n^8 + 24756*n^7 + 43974*n^6 + 65352*n^5
 *            * + 60795*n^4 + 81524*n^3 + 72036*n^2 + 1872*n) / 9!
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 17, u64 n <= 211, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Compute with 128‑bit integers to avoid overflow in the numerator. */
    unsigned __int128 n128 = n;
    unsigned __int128 n1   = n128;
    unsigned __int128 n2   = n1 * n128;
    unsigned __int128 n3   = n2 * n128;
    unsigned __int128 n4   = n3 * n128;
    unsigned __int128 n5   = n4 * n128;
    unsigned __int128 n6   = n5 * n128;
    unsigned __int128 n7   = n6 * n128;
    unsigned __int128 n8   = n7 * n128;
    unsigned __int128 n9   = n8 * n128;

    unsigned __int128 numer = 0;
    numer += (unsigned __int128)7936  * n9;
    numer += (unsigned __int128)4635  * n8;
    numer += (unsigned __int128)24756 * n7;
    numer += (unsigned __int128)43974 * n6;
    numer += (unsigned __int128)65352 * n5;
    numer += (unsigned __int128)60795 * n4;
    numer += (unsigned __int128)81524 * n3;
    numer += (unsigned __int128)72036 * n2;
    numer += (unsigned __int128)1872  * n1;

    numer /= 362880;          /* 9! = 362880 */

    return (seq_t)numer;
}
