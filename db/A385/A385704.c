/*
 * A385704  Complement of A184535.
 * https://oeis.org/A385704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+m if n+m < floor(3*(m+1)^2/5) a(n) = n+m+1 otherwise,
 *            where m = floor( sqrt( floor(5*n/3) ) )
 * Range:     u8 n <= 38, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* compute m = floor( sqrt( floor(5*n/3) ) ) */
    seq_t five = (seq_t)5;
    seq_t three = (seq_t)3;
    seq_t temp = five * (seq_t)n;          /* 5*n  */
    seq_t k = temp / three;                /* floor(5*n/3) */
    seq_t m = floor_sqrt(k);               /* floor( sqrt(k) ) */

    /* threshold = floor(3*(m+1)^2/5) */
    seq_t m_plus = m + (seq_t)1;
    seq_t m2 = m_plus * m_plus;
    seq_t thr = (three * m2) / five;

    /* sum = n + m */
    seq_t sum = (seq_t)n + m;

    /* return sum or sum+1 depending on the condition */
    return (sum >= thr) ? (sum + (seq_t)1) : sum;
}
