/*
 * A279112  Number of non-equivalent ways to place 3 non-attacking kings on an
 *          n X n board.
 * https://oeis.org/A279112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n = 1 a(n) = (n^6 - 27*n^4 + 44*n^3 + 146*n^2 -
 *            404*n + 240)/48 for even n >= 2 a(n) = (n^6 - 27*n^4 + 52*n^3 +
 *            125*n^2 - 388*n + 189)/48 for odd n >= 3 (values from the OEIS
 *            entry) This implementation uses 128‑bit arithmetic for the
 *            intermediate calculations to avoid unsigned underf...
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 76, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;          /* a(1) = 0 */

    /* 128‑bit integers for intermediate arithmetic */
    __int128 N  = n;
    __int128 N2 = N * N;     /* n^2 */
    __int128 N3 = N2 * N;    /* n^3 */
    __int128 N4 = N3 * N;    /* n^4 */
    __int128 N6 = N4 * N2;   /* n^6 */

    __int128 num;
    if ((N & 1) == 0) {          /* even n */
        num = N6 - 27 * N4 + 44 * N3 + 146 * N2 - 404 * N + 240;
    } else {                     /* odd n (n>=3) */
        num = N6 - 27 * N4 + 52 * N3 + 125 * N2 - 388 * N + 189;
    }

    /* division is exact */
    return (seq_t)(num / 48);
}
