/*
 * A290362  Number of 8-leaf rooted trees with n levels.
 * https://oeis.org/A290362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (272*n^7+273*n^6+749*n^5+1365*n^4+1043*n^3+882*n^2+456*n)/5040
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 35, u64 n <= 857, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the polynomial in a 128‑bit intermediate to avoid overflow
       on 64‑bit builds.  The final result always fits in seq_t for the
       values that fit in 64 bits, and it is exact for arbitrary‑precision
       builds. */
    __int128 tn = n;          /* idx_t to 128‑bit */
    __int128 n1 = tn;         /* n^1 */
    __int128 n2 = n1 * n1;    /* n^2 */
    __int128 n3 = n2 * n1;    /* n^3 */
    __int128 n4 = n3 * n1;    /* n^4 */
    __int128 n5 = n4 * n1;    /* n^5 */
    __int128 n6 = n5 * n1;    /* n^6 */
    __int128 n7 = n6 * n1;    /* n^7 */

    __int128 res = (__int128)272 * n7;
    res += (__int128)273 * n6;
    res += (__int128)749 * n5;
    res += (__int128)1365 * n4;
    res += (__int128)1043 * n3;
    res += (__int128)882 * n2;
    res += (__int128)456 * n1;
    res /= (__int128)5040;    /* 7! */

    return (seq_t)res;
}
