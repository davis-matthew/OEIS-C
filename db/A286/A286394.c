/*
 * A286394  Number of inequivalent n X n matrices over GF(8) under action of
 *          dihedral group of the square D_4.
 * https://oeis.org/A286394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/8)*(8^(n^2) + 2*8^(n^2/4) + 3*8^(n^2/2) +
 *            2*8^((n^2+n)/2)) for n even * a(n) = (1/8)*(8^(n^2) +
 *            2*8^((n^2+3)/4) + 8^((n^2+1)/2) + 4*8^((n^2+n)/2)) for n odd
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    seq_t total = 0;
    if ((n & 1) == 0) { /* n is even */
        idx_t n2 = n * n;
        seq_t p_n2          = pow_uint(8, n2);
        seq_t p_n2_div4     = pow_uint(8, n2 / 4);
        seq_t p_n2_div2     = pow_uint(8, n2 / 2);
        seq_t p_n2_n_div2   = pow_uint(8, (n2 + n) / 2);
        total = p_n2 + 2 * p_n2_div4 + 3 * p_n2_div2 + 2 * p_n2_n_div2;
    } else { /* n is odd */
        idx_t n2 = n * n;
        seq_t p_n2          = pow_uint(8, n2);
        seq_t p1            = pow_uint(8, (n2 + 3) / 4);
        seq_t p2            = pow_uint(8, (n2 + 1) / 2);
        seq_t p3            = pow_uint(8, (n2 + n) / 2);
        total = p_n2 + 2 * p1 + p2 + 4 * p3;
    }
    return total / 8;
}
