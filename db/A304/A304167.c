/*
 * A304167  a(n) = 3^n - 2^(n-1) + 2 (n>=1).
 * https://oeis.org/A304167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 2^(n-1) + 2 (n>=1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t val = pow3(n);          /* compute 3^n */
    val -= pow2(n - 1);           /* subtract 2^(n-1) */
    val += (seq_t)2;              /* add 2 */
    return val;
}
