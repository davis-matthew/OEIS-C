/*
 * A358358  Maximum length of an induced cycle (or chordless cycle) in the
 *          n-folded cube graph.
 * https://oeis.org/A358358
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 0 (n=2), a(3)=3, a(4)=4, and for n≥5 a(n)=3·2^(n-4)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)4;
    /* For n ≥ 5: a(n) = 3 * 2^(n-4) */
    seq_t res = pow2(n - 4);
    res = mul_small(res, 3);
    return res;
}
