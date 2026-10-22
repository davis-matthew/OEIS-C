/*
 * A332135  a(n) = (10^(2n+1)-1)/3 + 2*10^n.
 * https://oeis.org/A332135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^(2n+1) - 1)/3 + 2*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A332135 */
#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* compute 10^(2n+1) and 10^n */
    seq_t p2n1 = pow10(2*n + 1);
    seq_t pn   = pow10(n);

    /* (10^(2n+1) - 1) / 3 + 2*10^n */
    seq_t result = p2n1 - (seq_t)1;
    result /= (seq_t)3;
    result += (seq_t)2 * pn;

    return result;
}
