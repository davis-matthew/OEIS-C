/*
 * A352982  a(n) = Sum_{k=0..floor(n/3)} k^n.
 * https://oeis.org/A352982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} k^n
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_k = n / 3;

    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t term;
        if (k == 0) {
            term = (n == 0) ? (seq_t)1 : (seq_t)0;   /* 0^0 = 1, otherwise 0 */
        } else {
            term = pow_uint(k, n);
        }
        sum += term;
    }
    return sum;
}
