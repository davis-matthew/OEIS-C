/*
 * A332127  a(n) = 2*(10^(2n+1)-1)/9 + 5*10^n.
 * https://oeis.org/A332127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*(10^(2n+1)-1)/9 + 5*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* 10^n */
    seq_t pow10_n = pow10(n);

    /* 10^(2n+1) = 10^(2*n + 1) */
    idx_t exp2n1 = 2 * n + 1;
    seq_t pow10_2n1 = pow10(exp2n1);

    /* repunit of length 2n+1: (10^(2n+1) - 1) / 9 */
    seq_t repunit = (pow10_2n1 - 1) / 9;

    /* a(n) = 2*repunit + 5*10^n */
    return 2 * repunit + 5 * pow10_n;
}
