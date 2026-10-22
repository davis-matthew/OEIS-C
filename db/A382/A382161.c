/*
 * A382161  "Repunit" Kaprekar numbers.
 * https://oeis.org/A382161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^(9*n-8)-1)/9
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
 
seq_t a(idx_t n) {
    /* compute exponent: e = 9*n - 8 */
    idx_t e = 9*n - 8ull;
    seq_t p = pow10(e);
    /* (10^e - 1) / 9 */
    p = p - (seq_t)1;
    p = p / (seq_t)9;
    return p;
}
