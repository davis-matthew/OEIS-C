/*
 * A243142  Number of inequivalent (mod D_3) ways to place 3 points on a
 *          triangular grid of side n so that they are not vertices of an
 *          equilateral triangle of any orientation.
 * https://oeis.org/A243142
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + 3*n^5 - 5*n^4 + 6*n^3 - 68*n^2 + 72*n + IF(MOD(n,
 *            2) = 1)*(27*n^2 - 81*n + 45))/288
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 63, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Convert index to seq_t for arithmetic. */
    seq_t N = (seq_t)n;

    /* Powers of n. */
    seq_t n2 = N * N;           /* n^2   */
    seq_t n3 = n2 * N;          /* n^3   */
    seq_t n4 = n3 * N;          /* n^4   */
    seq_t n5 = n4 * N;          /* n^5   */
    seq_t n6 = n5 * N;          /* n^6   */

    /* Positive contributions. */
    seq_t pos = n6 + (3 * n5) + (6 * n3) + (72 * N);

    /* Negative contributions. */
    seq_t neg = (5 * n4) + (68 * n2);

    /* Parity adjustment (only for odd n). */
    seq_t parity = 0;
    if (n & 1) {
        parity = (27 * n2) - (81 * N) + 45;
    }

    /* Numerator = pos + parity - neg (guaranteed non‑negative). */
    seq_t numerator = pos + parity - neg;

    /* Divide by 288. 288 fits in seq_t. */
    return numerator / (seq_t)288;
}
