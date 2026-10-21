/*
 * A254030  a(n) = 1*4^n + 2*3^n + 3*2^n + 4*1^n.
 * https://oeis.org/A254030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1*4^n + 2*3^n + 3*2^n + 4*1^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   mul_small, pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    seq_t v4 = pow4(n);                  /* 4^n */
    seq_t v3 = pow3(n);                  /* 3^n */
    seq_t v2 = pow2(n);                  /* 2^n */

    seq_t result = v4;                   /* start with 1*4^n */
    result += mul_small(v3, 2);          /* add 2*3^n */
    result += mul_small(v2, 3);          /* add 3*2^n */
    result += (seq_t)4;                  /* add 4*1^n (always 4) */

    return result;
}
