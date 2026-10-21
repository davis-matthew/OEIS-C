/*
 * A280373  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 260", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A280373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-4) for n > 3; Initial values: a(0)=1, a(1)=2,
 *            a(2)=5, a(3)=8, a(4)=16, a(5)=40, a(6)=84, a(7)=130, a(8)=257,
 *            a(9)=640, a(10)=1344, a(11)=2080, a(12)=4096
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static const seq_t init[13] = {
        (seq_t)1,  (seq_t)2,  (seq_t)5,  (seq_t)8,
        (seq_t)16, (seq_t)40, (seq_t)84, (seq_t)130,
        (seq_t)257, (seq_t)640, (seq_t)1344, (seq_t)2080,
        (seq_t)4096
    };

    if (n < 13)
        return init[n];

    /* For n ≥ 13: a(n) = 16^k * a(n-4k) with n-4k < 13.
       Choose k = floor((n-9)/4). */
    idx_t k   = (n - 9) / 4;
    idx_t n0  = n - 4 * k;          /* n0 ∈ {9,10,11,12} so init[n0] exists */
    seq_t pw  = pow2(4 * k);         /* 16^k = 2^(4k) */
    return init[n0] * pw;
}
