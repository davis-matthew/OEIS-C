/*
 * A254646  Fourth partial sums of seventh powers (A001015).
 * https://oeis.org/A254646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n*(n+1)*(n+2)*(n+3)*(n+4)*(48-100*n-89*n^2+160*n^3+140*n^4+36*n^5+3*n^6)/23760
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 15, u64 n <= 125, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use unsigned __int128 for intermediate arithmetic to avoid overflow on 64‑bit seq_t. */
    unsigned __int128 nn  = n;
    unsigned __int128 nn2 = nn * nn;
    unsigned __int128 nn3 = nn2 * nn;
    unsigned __int128 nn4 = nn3 * nn;
    unsigned __int128 nn5 = nn4 * nn;
    unsigned __int128 nn6 = nn5 * nn;

    unsigned __int128 prod = nn * (nn + 1) * (nn + 2) * (nn + 3) * (nn + 4);

    unsigned __int128 poly = 48;
    poly -= 100 * nn;
    poly -= 89  * nn2;
    poly += 160 * nn3;
    poly += 140 * nn4;
    poly += 36  * nn5;
    poly +=  3  * nn6;

    unsigned __int128 numerator = prod * poly;
    unsigned __int128 result    = numerator / 23760;

    return (seq_t)result;
}
