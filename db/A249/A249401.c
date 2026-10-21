/*
 * A249401  a(n) = OP(sum{i=0,...,n} OP(binomial(n,i))), where OP(n) is the
 *          odd part of n (A000265).
 * https://oeis.org/A249401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = OP( sum_{i=0}^n OP( binomial(n,i) ) )
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/odd_part.h"

static inline idx_t trailing_zeros(idx_t x)
{
    return x == 0 ? 64 : __builtin_ctzll(x);
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;          /* OP(1) = 1 */
    }

    /* current odd part of binomial(n,i) (initially for i=0) */
    seq_t curr_odd = 1;
    /* sum of odd parts for all i up to current index */
    seq_t sum = 1;             /* include the i = 0 term */

    for (idx_t i = 0; i < n; ++i) {
        idx_t num = n - i;          /* n - i          */
        idx_t den = i + 1;          /* i + 1          */

        /* remove powers of 2 from num and den            */
        idx_t e_num = trailing_zeros(num);
        idx_t e_den = trailing_zeros(den);
        idx_t odd_num = num >> e_num;  /* odd part of num  */
        idx_t odd_den = den >> e_den;  /* odd part of den  */

        /* update odd part for next i (i+1) */
        curr_odd = curr_odd * (seq_t)odd_num;
        curr_odd = curr_odd / (seq_t)odd_den;

        /* add to the total sum of odd parts */
        sum += curr_odd;
    }

    /* return the odd part of the accumulated sum */
    return odd_part(sum);
}
