/*
 * A392517  a(n) is the minimum diameter of a n-element set of integers for
 *          which no nonzero d has more than d-1 representations as a
 *          difference of elements of the set.
 * https://oeis.org/A392517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(1)=0. For n>=2, let * k = ⌊(1+√(1+8*(n−2)))/2⌋, * offset_k = 2
 *            + (k−1)*k/2, * r = n − offset_k + 1, * then a(n) =
 *            ((k−1)*k*(k+1))/3 + r*(k+1). * This follows from the empirical
 *            observation that a(n+1)−a(n) equals A003057
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* compute k */
    idx_t temp = n - 2;                   /* n >= 2 */
    idx_t val  = 1 + 8 * temp;             /* 1 + 8*(n-2) */

    double d = sqrt((double)val);
    idx_t sqrt_val = (idx_t)d;
    /* Correct possible rounding errors */
    while ((sqrt_val + 1) * (sqrt_val + 1) <= val)
        ++sqrt_val;
    while (sqrt_val * sqrt_val > val)
        --sqrt_val;

    idx_t k = (1 + sqrt_val) / 2;          /* floor division by 2 */

    /* offset of the first element of block k */
    seq_t offset_k = 2 + (seq_t)(k - 1) * (seq_t)k / 2;
    seq_t pos_in_block = (seq_t)n - offset_k + 1;

    /* sum of full blocks before block k */
    seq_t t1 = (seq_t)(k - 1) * (seq_t)k;
    seq_t sum_full = (t1 * (seq_t)(k + 1)) / 3;

    seq_t total = sum_full + pos_in_block * (seq_t)(k + 1);
    return total;
}
