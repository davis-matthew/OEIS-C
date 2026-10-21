/*
 * A264791  Number of n X 1 arrays of permutations of 0..n*1-1 with rows
 *          nondecreasing modulo 2 and columns nondecreasing modulo 7.
 * https://oeis.org/A264791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=0..6} ⌊(n+i)/7⌋!
 * Range:     u8 n <= 14, u16 n <= 19, u32 n <= 27, u64 n <= 41, u128 n <= 62,
 *            big any n
 * Helpers:   factorial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factorial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 0; i < 7; ++i) {
        idx_t m = (n + i) / 7;          /* floor((n+i)/7) */
        seq_t f = factorial(m);         /* m!  */
        res = seq_mul(res, f);          /* res *= f  */
    }
    return res;
}
