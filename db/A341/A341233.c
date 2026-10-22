/*
 * A341233  Denominator of the expected fraction of guests without a napkin in
 *          Conway's napkin problem with n guests.
 * https://oeis.org/A341233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = denominator of the sum    
 *               S(n) = Σ_{k=2..n} (1 - 2^(2-k)) / k!
 * 
 * The procedure keeps a running reduced fraction
 * (sum_num / sum_den).  For each term k the
 * denominator term_den = k! · 2^(k-2) is a multiple of
 * the previous sum_den, so the new denominator can
 * be set directly to term_den.  The numerator is
 * updated with an integer multiplication that stays
 * within the range of the target type.
 *
 * Complexity:  O(n), with only simple integer operations.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1)              /* a(1) = 1 */
        return (seq_t)1;

    seq_t fact = 1;          /* k!   (start at 1, updated at loop start) */
    seq_t pow2 = 1;          /* 2^(k-2), initially 2^0 = 1 */

    seq_t sum_num = 0;       /* numerator of the partial sum */
    seq_t sum_den = 1;       /* denominator of the partial sum (always 1 or term_den) */

    for (idx_t k = 2; k <= n; ++k)
    {
        fact *= (seq_t)k;            /* k!      */

        /* term numerator and denominator for current k */
        seq_t term_num = pow2 - (seq_t)1;          /* 2^(k-2) - 1 */
        seq_t term_den = fact * pow2;              /* k! * 2^(k-2) */

        /* term_den is a multiple of sum_den. */
        seq_t factor = term_den / sum_den;        /* factor = k * 2 */

        /* new numerator over the common denominator term_den */
        seq_t new_num = sum_num * factor + term_num;
        seq_t new_den = term_den;

        /* reduce the fraction */
        seq_t g = gcd(new_num, new_den);
        sum_num = new_num / g;
        sum_den = new_den / g;

        /* prepare pow2 for next iteration: 2^(k-1) = 2^(k-2) * 2 */
        pow2 <<= 1;
    }

    return sum_den;           /* a(n) is the denominator of S(n) */
}
