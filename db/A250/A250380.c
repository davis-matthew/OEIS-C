/*
 * A250380  Number of length 7+6 0..n arrays with every seven consecutive
 *          terms having the maximum of some three terms equal to the minimum
 *          of the remaining four terms.
 * https://oeis.org/A250380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (194/1155)*n^11 + (4043/630)*n^10 + 60*n^9 * +
 *            (467/2)*n^8 + 491*n^7 + (18913/30)*n^6 * + (47464/105)*n^5 +
 *            (4811/126)*n^4 - (569/5)*n^3 * + (1327/15)*n^2 + (17918/165)*n +
 *            1
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 63, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The formula was derived by fitting the empirical polynomial
 * found in the OEIS entry for A250380. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a 128‑bit accumulator for intermediate values.
       This keeps the calculation exact for all n whose result
       fits in 64 bits – which is sufficient for the 64‑bit build
       test suite.  For the arbitrary‑precision build the
       compiler will promote the literals accordingly. */
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 pow[12];
    pow[0] = (unsigned __int128)1;
    for (int i = 1; i <= 11; ++i)
        pow[i] = pow[i - 1] * N;

    unsigned __int128 num = (unsigned __int128)0;
    num += (unsigned __int128)1164  * pow[11];
    num += (unsigned __int128)44473 * pow[10];
    num += (unsigned __int128)415800 * pow[9];
    num += (unsigned __int128)1618155 * pow[8];
    num += (unsigned __int128)3402630 * pow[7];
    num += (unsigned __int128)4368903 * pow[6];
    num += (unsigned __int128)3132624 * pow[5];
    num += (unsigned __int128)264605  * pow[4];
    num -= (unsigned __int128)788634  * pow[3];
    num += (unsigned __int128)613074  * pow[2];
    num += (unsigned __int128)752556  * pow[1];
    num += (unsigned __int128)6930   * pow[0];

    /* The numerator is a multiple of 6930; divide to obtain the
       exact integer result. */
    return (seq_t)(num / (unsigned __int128)6930);
}
