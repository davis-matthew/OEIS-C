/*
 * A256320  Number of partitions of 4n into exactly 3 parts.
 * https://oeis.org/A256320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(6*n^2+cos(2*Pi*n/3)-1)/9 * Equivalent integer formula:
 *            * if n % 3 == 0 -> a(n) = (4*n^2)/3 * otherwise -> a(n) = (4*n^2
 *            - 1)/3 * This implements the OEIS sequence A256320
 * Range:     u8 n <= 7, u16 n <= 129, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert to the wider seq_t type before arithmetic. */
    seq_t nn = (seq_t)n;
    seq_t four_nn = 4 * nn * nn;   /* 4*n^2 */

    if (n % 3 == 0) {
        return four_nn / 3;            /* (4*n^2)/3 exact division */
    } else {
        return (four_nn - 1) / 3;     /* (4*n^2 - 1)/3 exact division */
    }
}
