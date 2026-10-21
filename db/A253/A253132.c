/*
 * A253132  Number of length 4+2 0..n arrays with the sum of medians of
 *          adjacent triples multiplied by some arrangement of +-1 equal to
 *          zero.
 * https://oeis.org/A253132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2/15)n^6 + (1273/405)n^5 + (298/27)n^4 + (601/81)n^3 +
 *            (143/15)n^2 + (238/45)n + 1 if n mod 3 = 0 a(n) = (2/15)n^6 +
 *            (1273/405)n^5 + (298/27)n^4 + (203/27)n^3 + (4001/405)n^2 +
 *            (32/5)n + 17/9 if n mod 3 = 1 a(n) = (2/15)n^6 + (1273/405)n^5 +
 *            (298/27)n^4 + (601/81)n^3 + (3781/405)n^2 + (21...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 52, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute powers of n up to n^6 using unsigned __int128
       to avoid overflow for n up to at least 10 000. */
    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 n2   = n128 * n128;
    unsigned __int128 n3   = n2   * n128;
    unsigned __int128 n4   = n3   * n128;
    unsigned __int128 n5   = n4   * n128;
    unsigned __int128 n6   = n5   * n128;

    unsigned __int128 numer = 0;
    switch (n % 3) {
        case 0:
            numer = (unsigned __int128)54  * n6
                   + (unsigned __int128)1273 * n5
                   + (unsigned __int128)4470 * n4
                   + (unsigned __int128)3005 * n3
                   + (unsigned __int128)3861 * n2
                   + (unsigned __int128)2142 * n128
                   + 405;
            break;
        case 1:
            numer = (unsigned __int128)54   * n6
                   + (unsigned __int128)1273 * n5
                   + (unsigned __int128)4470 * n4
                   + (unsigned __int128)3045 * n3
                   + (unsigned __int128)4001 * n2
                   + (unsigned __int128)2592 * n128
                   + 765;
            break;
        case 2:
            numer = (unsigned __int128)54   * n6
                   + (unsigned __int128)1273 * n5
                   + (unsigned __int128)4470 * n4
                   + (unsigned __int128)3005 * n3
                   + (unsigned __int128)3781 * n2
                   + (unsigned __int128)1962 * n128
                   + 365;
            break;
    }
    /* Result is the numerator divided by the common denominator 405. */
    unsigned long long result_ll = (unsigned long long)(numer / 405);
    return (seq_t)result_ll;
}
