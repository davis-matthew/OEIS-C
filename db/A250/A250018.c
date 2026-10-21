/*
 * A250018  Number of length 4+5 0..n arrays with no six consecutive terms
 *          having the maximum of any three terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A250018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^9 + (27/70)*n^8 + (146/35)*n^7 + (29/10)*n^6 +
 *            (36/5)*n^5 + (29/10)*n^4 - (14/15)*n^3 + (267/70)*n^2 -
 *            (151/105)*n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* use unsigned __int128 for intermediate calculations */
    unsigned __int128 N = (unsigned __int128)n;

    unsigned __int128 p2 = N * N;
    unsigned __int128 p3 = p2 * N;
    unsigned __int128 p4 = p3 * N;
    unsigned __int128 p5 = p4 * N;
    unsigned __int128 p6 = p5 * N;
    unsigned __int128 p7 = p6 * N;
    unsigned __int128 p8 = p7 * N;
    unsigned __int128 p9 = p8 * N;

    /* integerized coefficients multiplied by 210 (LCM of denominators) */
    const unsigned __int128 C9 = (unsigned __int128)210;
    const unsigned __int128 C8 = (unsigned __int128)81;
    const unsigned __int128 C7 = (unsigned __int128)876;
    const unsigned __int128 C6 = (unsigned __int128)609;
    const unsigned __int128 C5 = (unsigned __int128)1512;
    const unsigned __int128 C4 = (unsigned __int128)609;
    const unsigned __int128 C3 = (unsigned __int128)196;   /* subtracted */
    const unsigned __int128 C2 = (unsigned __int128)801;
    const unsigned __int128 C1 = (unsigned __int128)302;   /* subtracted */

    unsigned __int128 f =
        C9 * p9
      + C8 * p8
      + C7 * p7
      + C6 * p6
      + C5 * p5
      + C4 * p4
      - C3 * p3
      + C2 * p2
      - C1 * N;

    /* divide by 210 to obtain the exact integer value */
    return (seq_t)(f / 210);
}
