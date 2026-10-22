/*
 * A341549  a(n) = Sum_{k=1..n} (-1)^(n+k)*A087322(n,k).
 * https://oeis.org/A341549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n + 1)*2^(n + 2) - 3 + (4*n + 1)*(-1)^(n + 1))/6
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* offset is 1; the sequence is undefined for n = 0 but return 0 for safety */
    if (n == 0)
        return 0;

    /* 2^(n+2) as a seq_t */
    seq_t pow = pow2(n + 2);

    /* (n + 1) * 2^(n + 2) */
    seq_t part1 = (seq_t)(n + 1) * pow;

    /* Compute (4*n + 1)*(-1)^(n + 1) by handling the sign separately */
    seq_t part2_pos = 0;  /* when the term is positive  */
    seq_t part2_neg = 0;  /* when the term is negative  */

    if ((n & 1ULL) == 0) /* n even → (-1)^(n+1) = -1 */
        part2_neg = (seq_t)(4 * n + 1);
    else                 /* n odd  → (-1)^(n+1) =  1 */
        part2_pos = (seq_t)(4 * n + 1);

    /* Numerator: part1 + part2_pos - 3 - part2_neg */
    seq_t num = part1 + part2_pos;
    num -= (seq_t)3;
    if (part2_neg)
        num -= part2_neg;

    /* Divide by 6 (the result is always an integer) */
    return num / 6;
}
