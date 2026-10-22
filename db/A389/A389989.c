/*
 * A389989  a(n) = 3^n - 2^n - 2*n - 1, n > 1; a(0)=a(1)=0.
 * https://oeis.org/A389989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 2^n - 2*n - 1 (for n>1), a(0)=a(1)=0
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    if (n < 2)
        return (seq_t)0;
    seq_t p3 = pow3(n);
    seq_t p2 = pow2(n);
    seq_t two_n = (seq_t)2 * (seq_t)n;
    seq_t res = p3 - p2 - two_n - (seq_t)1;
    return res;
}
