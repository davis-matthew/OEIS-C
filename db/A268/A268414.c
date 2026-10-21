/*
 * A268414  a(n) = 5*a(n-1) - 2*n for n > 0, a(0) = 1.
 * https://oeis.org/A268414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 5*a(n-1) - 2*n for n>0; Closed form: a(n) =
 *            (4*n + 3*5^n + 5)/8
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow_seq5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/pow_seq5.h"

seq_t a(idx_t n) {
    seq_t pow5 = pow_seq5(n);
    seq_t term = 4 * ((seq_t)n) + 3 * pow5 + 5;
    return term / 8;
}
