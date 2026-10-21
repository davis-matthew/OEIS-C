/*
 * A283029  Number of inequivalent 5 X 5 matrices with entries in {1,2,3,..,n}
 *          when a matrix and its transpose are considered equivalent.
 * https://oeis.org/A283029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^15*(n^2+1)*(n^8-n^6+n^4-n^2+1)/2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0)=0, a(1)=1*/
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t N = (seq_t)n;                 /* base value as seq_t        */
    unsigned __int128 n1 = (unsigned __int128)N;
    unsigned __int128 n2 = n1 * n1;                     /* n^2            */
    unsigned __int128 n3 = n2 * n1;                     /* n^3            */
    unsigned __int128 n4 = n2 * n2;                     /* n^4            */
    unsigned __int128 n6 = n3 * n3;                     /* n^6            */
    unsigned __int128 n8 = n4 * n4;                     /* n^8            */
    unsigned __int128 n15 = 1;                          /* will build n^15 */

    /* n^15 = n^8 * n^4 * n^3 */
    n15 = n8 * n4 * n3;

    unsigned __int128 factor1 = n2 + 1; /* n^2 + 1          */
    unsigned __int128 factor2 = n8 - n6 + n4 - n2 + 1; /* n^8-n^6+n^4-n^2+1 */

    unsigned __int128 prod = n15 * factor1;
    prod = prod * factor2;
    prod /= 2;

    return (seq_t)prod;
}
