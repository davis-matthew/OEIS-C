/*
 * A272647  a(n) = A001517(n) mod 7.
 * https://oeis.org/A272647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*m^6 - 54*m^5 + 365*m^4 - 1140*m^3 + 1582*m^2 - 636*m +
 *            60)/60, * where m = n mod 7. This computes A001517(n) modulo 7
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Reduce the index modulo 7. */
    idx_t m = n % 7u;                 /* 0 <= m < 7 */

    /* Work in signed 64‑bit to avoid overflow of the polynomial terms. */
    int64_t mm  = (int64_t)m;
    int64_t m2  = mm * mm;             /* m^2 */
    int64_t m3  = m2 * mm;             /* m^3 */
    int64_t m4  = m3 * mm;             /* m^4 */
    int64_t m5  = m4 * mm;             /* m^5 */
    int64_t m6  = m5 * mm;             /* m^6 */

    int64_t num = 3  * m6
                - 54 * m5
                + 365 * m4
                - 1140 * m3
                + 1582 * m2
                - 636 * mm
                + 60;

    /* The result of the division is always an integer in [0,6]. */
    return (seq_t)(num / 60);
}
