/*
 * A243647  Number of ways four L-tiles can be placed on an n X n square.
 * https://oeis.org/A243647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 -8*n^7 -14*n^6 +244*n^5 -201*n^4 -2428*n^3 +4042*n^2
 *            +7700*n -15576) / 24 for n >= 4 a(n) = 0 for n < 4
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 25, u64 n <= 381, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;

    __int128 dn = n;
    __int128 n2 = dn * dn;
    __int128 n3 = n2 * dn;
    __int128 n4 = n3 * dn;
    __int128 n5 = n4 * dn;
    __int128 n6 = n5 * dn;
    __int128 n7 = n6 * dn;
    __int128 n8 = n7 * dn;

    __int128 val = n8
                   - (__int128)8  * n7
                   - (__int128)14 * n6
                   + (__int128)244 * n5
                   - (__int128)201 * n4
                   - (__int128)2428 * n3
                   + (__int128)4042 * n2
                   + (__int128)7700 * dn
                   - (__int128)15576;

    val /= 24;

    return (seq_t)val;
}
