/*
 * A271771  Maximum total Hamming distance between pairs of consecutive
 *          elements in any permutation of all 2^n binary words of length n.
 * https://oeis.org/A271771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*2^n - 2^(n-1) - n + 1
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t p2_n          = pow2(n);
    seq_t p2_n_minus_1  = pow2(n - 1);
    seq_t result        = p2_n * (seq_t)n - p2_n_minus_1 - (seq_t)n + 1;
    return result;
}
