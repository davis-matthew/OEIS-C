/*
 * A250143  Number of length 4+2 0..n arrays with the medians of every three
 *          consecutive terms nondecreasing.
 * https://oeis.org/A250143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (104*n^6 + 900*n^5 + 2975*n^4 + 4770*n^3 + 3941*n^2 +
 *            1710*n + 360) / 360
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 48, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For the offset of the sequence, a(0) is undefined.
       The function assumes n >= 1 as per the OEIS definition. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Use an unsigned 128‑bit integer for intermediate computations
       to avoid overflow for the 64‑bit build while still being valid
       in the arbitrary‑precision build.  */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n2 = nn * nn;          /* n² */
    unsigned __int128 n3 = n2 * nn;          /* n³ */
    unsigned __int128 n4 = n3 * nn;          /* n⁴ */
    unsigned __int128 n5 = n4 * nn;          /* n⁵ */
    unsigned __int128 n6 = n5 * nn;          /* n⁶ */

    unsigned __int128 numer = (unsigned __int128)104 * n6;
    numer += (unsigned __int128)900 * n5;
    numer += (unsigned __int128)2975 * n4;
    numer += (unsigned __int128)4770 * n3;
    numer += (unsigned __int128)3941 * n2;
    numer += (unsigned __int128)1710 * nn;
    numer += 360;

    return (seq_t)(numer / 360);
}
