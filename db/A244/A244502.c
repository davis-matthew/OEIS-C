/*
 * A244502  Number of ways to place 4 points on an n X n X n triangular grid
 *          so that no pair of them has distance sqrt(3).
 * https://oeis.org/A244502
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = (n^8 + 4*n^7 - 78*n^6 + 40*n^5 + 2625*n^4 * - 10172*n^3
 *            - 16564*n^2 + 180432*n - 321408) / 384 * for n >= 5. The
 *            provided sequence also has a(4) = 33. * The coefficients come
 *            from the rational polynomial in the OEIS * entry; they are
 *            multiplied by 384 so that the denominator is 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 33, u64 n <= 538, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 4) {
        return 0;           /* value undefined for n<4; return 0 */
    }
    if (n == 4) {
        return 33;          /* a(4) is defined explicitly */
    }

    /* Use unsigned __int128 for intermediate calculations so that
     * n^8 fits comfortably (n <= 10000 implies n^8 < 10^32 < 2^128). */
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n2 * n2;
    unsigned __int128 n5 = n4 * N;
    unsigned __int128 n6 = n3 * n3;     /* n^6  */
    unsigned __int128 n7 = n6 * N;
    unsigned __int128 n8 = n4 * n4;     /* n^8  */

    unsigned __int128 num = 0;
    num += n8;                          /* + 1 * n^8               */
    num += 4 * n7;                      /* + 4 * n^7               */
    num -= 78 * n6;                     /* - 78 * n^6              */
    num += 40 * n5;                     /* + 40 * n^5              */
    num += 2625 * n4;                   /* + 2625 * n^4            */
    num -= 10172 * n3;                  /* - 10172 * n^3           */
    num -= 16564 * n2;                  /* - 16564 * n^2           */
    num += 180432 * N;                  /* + 180432 * n            */
    num -= 321408;                      /* - 321408                */

    /* The numerator is guaranteed to be divisible by 384 for n >= 5. */
    return (seq_t)(num / 384);
}
